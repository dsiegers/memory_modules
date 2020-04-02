#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>

 
 
#define WR_PAGE _IOW('a','a',struct page*)
#define RD_PAGE _IOR('a','b',struct page*)


DECLARE_PER_CPU(unsigned long int, freelistCounter);
DECLARE_PER_CPU(unsigned long int, freelistIf);
DECLARE_PER_CPU(unsigned long int, freelistEndif);
DECLARE_PER_CPU(unsigned long int, freelistTry);

 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

unsigned long int check = 18000000000000000000;
unsigned long int fix_loop = 18446744073709551615;
unsigned long int freelist, if_start, endif_start, try_start;

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

struct my_data
{
        struct page *current_page;
        u64 time;
	unsigned long int freelist_inc, if_time, endif_time, try_time; 
};

struct my_data data;

 
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = etx_read,
        .write          = etx_write,
        .open           = etx_open,
        .unlocked_ioctl = etx_ioctl,
        .release        = etx_release,
};
 
static int etx_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}
 
static int etx_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}
 
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Read Function\n");
        return 0;
}
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Write function\n");
        return 0;
}

 
static void get_per_cpu(void){
	freelist = get_cpu_var(freelistCounter);
	if_start = get_cpu_var(freelistIf);
	endif_start = get_cpu_var(freelistEndif);
	try_start = get_cpu_var(freelistTry);
}

static void compare_per_cpu(void){
	data.freelist_inc = get_cpu_var(freelistCounter) - freelist;
	if (data.freelist_inc > check) {
                data.freelist_inc = fix_loop - data.freelist_inc;
        }
	data.if_time = get_cpu_var(freelistIf) - if_start;
	if (data.if_time > check) {
                data.if_time = fix_loop - data.if_time;
        } 
        data.endif_time = get_cpu_var(freelistEndif) - endif_start;
	if (data.endif_time > check) {
                data.endif_time = fix_loop - data.endif_time;
        }
        data.try_time = get_cpu_var(freelistTry) - try_start;
	if (data.try_time > check) {
                data.try_time = fix_loop - data.try_time;
        }
}

static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	u64 start_time;
	u64 end_time;

        switch(cmd) {
                case WR_PAGE:
                        copy_from_user(&data.current_page ,(struct my_data*) arg, sizeof(data));
                        __free_pages(data.current_page, 6);
                        break;
                case RD_PAGE:
			get_per_cpu();
			start_time = ktime_get_ns();
			data.current_page = alloc_pages(GFP_KERNEL, 6);
			end_time = ktime_get_ns();
			data.time = end_time - start_time;
			compare_per_cpu();
                        copy_to_user((struct my_data*) arg, &data.current_page, sizeof(data));
		        break;
        }
        return 0;
}
 
 
static int __init etx_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&etx_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&etx_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"etx_class")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"etx_device")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}
 
void __exit etx_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DSiegers");
MODULE_DESCRIPTION("A simple device driver for kernel function timing");
MODULE_VERSION("1.0");
