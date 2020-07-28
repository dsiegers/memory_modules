#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <math.h>


#define WR_PAGE _IOW('a','a',struct page*)
#define RD_PAGE _IOR('a','b',struct page*)

struct my_data
{
        struct page *current_page;
        unsigned long time;
	unsigned long int freelist_inc, if_time, endif_time, try_time;
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
	while (get_meminfo()>1000) {
		if (j==size-1) {
			size = size*2;
			pages = realloc(pages, size*sizeof(struct page*));
		}
		ioctl(fd, RD_PAGE, (struct my_data*) data);
		pages[j++] = data->current_page;
		if (data->freelist_inc > 18000000000000000000) {
                        data->freelist_inc = pow(2,64)-1 - data->freelist_inc;
                }
		if (data->if_time > 18000000000000000000) {
       		        data->if_time = pow(2,64)-1 - data->if_time;
        	}
		if (data->endif_time > 18000000000000000000) {
                        data->endif_time = pow(2,64)-1 - data->endif_time;
                }
		if (data->try_time > 18000000000000000000) {
                        data->try_time = pow(2,64)-1 - data->try_time;
                }	
		fprintf(f, "%lu, %lu, %lu, %lu, %lu\n", data->time, data->freelist_inc, data->if_time, data->endif_time, data->try_time);
        	fflush(f);
		
		if (j%10 == 0) {
                	data->current_page = pages[j-10];
			ioctl(fd, WR_PAGE, (struct my_data*) data);
		}
	}
	fprintf(f, "done");
	fflush(f);
	fclose(f);
	while (j>0) {
		if (--j%10 != 0) {
			data->current_page = pages[j];
			ioctl(fd, WR_PAGE, (struct my_data*) data);
		}
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
