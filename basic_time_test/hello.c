#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lakshmanan");
MODULE_DESCRIPTION("A Simple Hello World module");

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world!\n");

    u64 start_time;
    u64  end_time;
    start_time = ktime_get_ns();
    /* the function you want to measure */
    
    *page test_page = alloc_pages(1,2);
    end_time = ktime_get_ns();
    printk("Elapsed time = %lld nanosecond\n", end_time - start_time);
    
    free(test_page);

    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
