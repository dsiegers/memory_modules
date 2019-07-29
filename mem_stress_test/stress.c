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



void recur_mem_process(int i){
    struct page *current_page = alloc_pages(GFP_KERNEL, 5);
//    if (read_file("/proc/meminfo")>0)
    if (i<1000)
        recur_mem_process(++i);
    __free_pages(current_page, 5);
    i--;
}



static int __init hello_init(void)
{
    int i=0;

    printk(KERN_INFO "Stress mod installed\n");

    recur_mem_process(i);

    printk(KERN_INFO "pages de-allocated");

    printk(KERN_INFO "Freelist CPU 1: %li", per_cpu(freelistCounter, 1));
    printk(KERN_INFO "Freelist CPU 2: %li", per_cpu(freelistCounter, 2));
    printk(KERN_INFO "Freelist CPU 3: %li", per_cpu(freelistCounter, 3));
    printk(KERN_INFO "Freelist CPU 4: %li", per_cpu(freelistCounter, 4));

    printk(KERN_INFO "Compact CPU 1: %li", per_cpu(compactCounter, 1));
    printk(KERN_INFO "Compact CPU 2: %li", per_cpu(compactCounter, 2));
    printk(KERN_INFO "Compact CPU 3: %li", per_cpu(compactCounter, 3));
    printk(KERN_INFO "Compact CPU 4: %li", per_cpu(compactCounter, 4));

    printk(KERN_INFO "Reclaim CPU 1: %li", per_cpu(reclaimCounter, 1));
    printk(KERN_INFO "Reclaim CPU 2: %li", per_cpu(reclaimCounter, 2));
    printk(KERN_INFO "Reclaim CPU 3: %li", per_cpu(reclaimCounter, 3));
    printk(KERN_INFO "Reclaim CPU 4: %li", per_cpu(reclaimCounter, 4));

    printk(KERN_INFO "cpuset CPU 1: %li", per_cpu(cpusetCounter, 1));
    printk(KERN_INFO "cpuset CPU 2: %li", per_cpu(cpusetCounter, 2));
    printk(KERN_INFO "cpuset CPU 3: %li", per_cpu(cpusetCounter, 3));
    printk(KERN_INFO "cpuset CPU 4: %li", per_cpu(cpusetCounter, 4));

    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
