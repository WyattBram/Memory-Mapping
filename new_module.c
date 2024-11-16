#include <linux/module.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kthread.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/slab.h>

static struct proc_dir_entry *tempdir, *tempinfo;
static unsigned char *buffer;
static unsigned char array[12]={0,1,2,3,4,5,6,7,8,9,10,11};

static void allocate_memory(void);
static void clear_memory(void);
static int my_map(struct file *filp, struct vm_area_struct *vma);


static const struct file_operations myproc_fops = {
        .mmap   = my_map,
};


static int my_map(struct file *filp, struct vm_area_struct *vma)
{
   unsigned long length = vma->vm_end - vma->vm_start;
    
    remap_pfn_range(vma, vma->vm_start, virt_to_phys(buffer) >> PAGE_SHIFT, 
                        length, vma->vm_page_prot);
    return 0;
}



static int init_myproc_module(void)
{
        tempdir=proc_mkdir("mydir", NULL);
        if(tempdir == NULL) {
                printk("mydir is NULL\n");
                return -ENOMEM;
        }

        tempinfo = proc_create("myinfo", 0666, tempdir, &myproc_fops);
        if(tempinfo == NULL) {
                printk("myinfo is NULL\n");
                remove_proc_entry("mydir",NULL);
                return -ENOMEM;
        }
        printk("init myproc module successfully\n");

        allocate_memory();

        return 0;
}


static void allocate_memory(void)
{
        buffer = (unsigned char *)kmalloc(PAGE_SIZE,GFP_KERNEL);
        memcpy(buffer, array, sizeof(array));
        SetPageReserved(virt_to_page(buffer));
}

static void clear_memory(void)
{
        ClearPageReserved(virt_to_page(buffer));
        kfree(buffer);
}


static void exit_myproc_module(void)
{
        clear_memory();
        remove_proc_entry("myinfo", tempdir);
        remove_proc_entry("mydir", NULL);
        printk("remove myproc module successfully\n");
}

module_init(init_myproc_module);
module_exit(exit_myproc_module);
MODULE_LICENSE("GPL");
