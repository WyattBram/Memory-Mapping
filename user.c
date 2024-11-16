#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define PAGE_SIZE 4096

int main(int argc , char *argv[])
{
        int fd;
        int i;
        unsigned char *p_map;

        fd = open("/proc/mydir/myinfo", O_RDWR);
        if(fd < 0) {
                printf("open fail\n");
                exit(1);
        }else {
                printf("open successfully by Wyatt\n");
        }


	p_map = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);


	for (int i = 0; i < 12; i++) {
	    printf("%d\n", p_map[i]);
	}

	close(fd);
	munmap(p_map, 4096);


        printf("Printed by Wyatt\n");


    return 0;
}
