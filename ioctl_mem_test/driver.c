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
#define PRINT_VARS _IO('a','c')

DECLARE_PER_CPU(unsigned long int, freelistCounter);
DECLARE_PER_CPU(unsigned long int, compactCounter);
DECLARE_PER_CPU(unsigned long int, reclaimCounter);
DECLARE_PER_CPU(unsigned long int, cpusetCounter);

 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
 
long int freelist, compact, reclaim, cpuset;

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
	int freelist_inc, compact_inc, reclaim_inc, cpuset_inc; 
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
	compact = get_cpu_var(compactCounter);
	reclaim = get_cpu_var(reclaimCounter);
	cpuset = get_cpu_var(cpusetCounter);
}

static void compare_per_cpu(void){
	data.freelist_inc = get_cpu_var(freelistCounter) - freelist;
	data.compact_inc = get_cpu_var(compactCounter) - compact;
	data.reclaim_inc = get_cpu_var(reclaimCounter) - reclaim;
	data.cpuset_inc = get_cpu_var(cpusetCounter) - cpuset;
/*
	data.freelist_inc = data.compact_inc = data.reclaim_inc = data.cpuset_inc = 0;
	if (get_cpu_var(freelistCounter)!=freelist)
		data.freelist_inc = 1;

	if (get_cpu_var(compactCounter)!=compact)
                data.compact_inc = 1;

	if (get_cpu_var(reclaimCounter)!=reclaim)
                data.reclaim_inc = 1;

	if (get_cpu_var(cpusetCounter)!=cpuset)
                data.cpuset_inc = 1;
*/
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
		case PRINT_VARS:
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
MODULE_DESCRIPTION("A simple device driver");
MODULE_VERSION("1.0");
