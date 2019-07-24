#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros



#include <asm/unistd.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/syscalls.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("DSiegers");
MODULE_DESCRIPTION("Memory Stress test");


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



static int __init hello_init(void)
{
    int size = 20;
    int i=0;
    struct page *current_page[size];

    printk(KERN_INFO "Stress mod installed\n");

    current_page[i] = alloc_pages(GFP_KERNEL, 2);
    
    __free_pages(current_page[i], 2);
    printk(KERN_INFO "pages de-allocated");
    
    read_file("/proc/meminfo");

    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
