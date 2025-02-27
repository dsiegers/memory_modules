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
		if (memFree <210000)
			printf("memFree = %li\n", memFree);
                return memFree;
        }
}

/*

void recur_mem_process(int i, int fd) {
	FILE *f = fopen("~/Documents/modules/ioctl_mem_test/time_data.csv", "w");
	struct my_data data;
//	alloc_pages(GFP_KERNEL, 5);
	ioctl(fd, RD_PAGE, (struct my_data*) &data);
	fprintf(f, "%li, %d, %d, %d, %d\n", data.time, data.freelist_inc, data.compact_inc, data.reclaim_inc, data.cpuset_inc);
	fflush(f);
	if (get_meminfo()>10000000);
    		recur_mem_process(++i, fd);
//	__free_pages(current_page, 5);
	ioctl(fd, WR_PAGE, (struct my_data*) &data);
	i--;
	fclose(f);
}
*/


void array_mem_process(int fd) {
	unlink("time_data.csv");
	FILE *f = fopen("time_data.csv", "a");
	if (!f){
		perror("fopen failed");
		return;
	}
	struct my_data *data = malloc (sizeof (struct my_data));
	int j = 0;
	int size = 1000;
	struct page* *pages = malloc(size*sizeof(struct page*));
	while (get_meminfo()>200000) {
		if (j==size-1) {
			size = size*2;
			pages = realloc(pages, size*sizeof(struct page*));
		}
		ioctl(fd, RD_PAGE, (struct my_data*) data);
		pages[j++] = data->current_page;
		fprintf(f, "%li, %d, %d, %d, %d\n", data->time, data->freelist_inc, data->compact_inc, data->reclaim_inc, data->cpuset_inc);
        	fflush(f);
	}
	fprintf(f, "done");
	fflush(f);
	fclose(f);
	while (j>0) {
		data->current_page = pages[--j];
		ioctl(fd, WR_PAGE, (struct my_data*) data);
	}
}


int main() {
	int i = 0;
	int fd;

	fd = open("/dev/etx_device", O_RDWR);
	if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
	array_mem_process(fd);

	close(fd);

	return 0;
}
