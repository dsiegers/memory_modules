#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros


#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/syscalls.h>

#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DSiegers");
MODULE_DESCRIPTION("Memory Stress test");

DECLARE_PER_CPU(unsigned long int, freelistCounter);
DECLARE_PER_CPU(unsigned long int, compactCounter);
DECLARE_PER_CPU(unsigned long int, reclaimCounter);
DECLARE_PER_CPU(unsigned long int, cpusetCounter);

static void read_file(char *filename)
{
  int fd;
  char buf[1];

  mm_segment_t old_fs = get_fs();
  set_fs(KERNEL_DS);

  fd = sys_open(filename, O_RDONLY, 0);
  if (fd >= 0) {
    printk(KERN_DEBUG);
    while (sys_read(fd, buf, 1) == 1){
    	printk("%c", buf[0]);
    	printk("\n");
    }
    sys_close(fd);
  }
  set_fs(old_fs);
}


void recur_mem_process(int i, struct page *current_page[]){
    current_page[i] = alloc_pages(GFP_KERNEL, 3);
    if (read_file("/proc/meminfo")>0)
	recur_mem_process(++i, current_page);
    __free_page(current_page[i], 3);
    if (i>0)
	i--;
}

static int __init hello_init(void)
{
    int i=0;
    struct page *current_page[20];

    printk(KERN_INFO "Stress mod installed\n");

    recur_mem_process(i, current_page);

    printk(KERN_INFO "pages de-allocated");

    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(freelistCounter, 1);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(freelistCounter, 2);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(freelistCounter, 3);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(freelistCounter, 4);

    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(compactCounter, 1);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(compactCounter, 2);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(compactCounter, 3);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(compactCounter, 4);

    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(reclaimCounter, 1);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(reclaimCounter, 2);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(reclaimCounter, 3);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(reclaimCounter, 4);

    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(cpusetCounter, 1);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(cpusetCounter, 2);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(cpusetCounter, 3);
    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(cpusetCounter, 4);

    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
