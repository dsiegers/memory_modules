#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


#define WR_PAGE _IOW('a','a',struct page*)
#define RD_PAGE _IOR('a','b',struct page*)

long int get_meminfo() {
        FILE *pf;
        long int memFree;

        pf = fopen("/proc/meminfo", "r");

        if (pf == NULL) {
                printf("Unable to open the file\n");
		return 0;
        } else {
                fscanf(pf, " MemTotal: %li kB ", &memFree);
                fscanf(pf, "MemFree: %li ", &memFree);
		fclose(pf);
                return memFree;
        }
}


void recur_mem_process(int i, int fd) {
	struct page *current_page;
//	alloc_pages(GFP_KERNEL, 5);
	ioctl(fd, RD_PAGE, (struct page*) &current_page);
	if (get_meminfo()>10000000);
    		recur_mem_process(++i, fd);
//	__free_pages(current_page, 5);
	ioctl(fd, WR_PAGE, (struct page*) &current_page);
	i--;
}




int main() {
	int i = 0;
	int fd;

	fd = open("/dev/etx_device", O_RDWR);
	if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
	recur_mem_process(i, fd);

	close(fd);

	return 0;
}
