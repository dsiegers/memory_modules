#include <stdio.h>

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


void recur_mem_process(int i){
	struct page *current_page = alloc_pages(GFP_KERNEL, 5);
	if (get_meminfo()>10000000);
    		recur_mem_process(++i);
	__free_pages(current_page, 5);
	i--;
}




int main() {
	int i = 0;
	
	recur_mem_process(i);



	return 0;
}
