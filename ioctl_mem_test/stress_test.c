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

struct my_data
{
        struct page *current_page;
        unsigned long time;
	int freelist_inc, compact_inc, reclaim_inc, cpuset_inc;
};


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
//	FILE *f = fopen("~/Documents/modules/ioctl_mem_test/time_data.csv", "w");
	struct my_data data;
//	alloc_pages(GFP_KERNEL, 5);
	ioctl(fd, RD_PAGE, (struct my_data*) &data);
//	fprintf(f, "%li, %d, %d, %d, %d\n", data.time, data.freelist_inc, data.compact_inc, data.reclaim_inc, data.cpuset_inc);
//	fflush(f);
	if (get_meminfo()>10000000);
    		recur_mem_process(++i, fd);
//	__free_pages(current_page, 5);
	ioctl(fd, WR_PAGE, (struct my_data*) &data);
	i--;
//	fclose(f);
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
