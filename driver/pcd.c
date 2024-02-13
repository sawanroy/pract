#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#define DEV_MEM_SIZE 512

#undef pr_fmt
#define pr_fmt(fmt) "%s : "fmt,__func__

char device_buf[DEV_MEM_SIZE];


dev_t device_num;

struct cdev pcd_cdev;

loff_t pcd_lseek(struct file *flip, loff_t offset, int whence){
    return 0;
}
ssize_t pcd_read(struct file *flip, char __user *buff, size_t count, loff_t *f_pos){
    return 0;
}
ssize_t pcd_write(struct file *flip, const char __user *buff, size_t count, loff_t *f_pos){
    return 0;
}
int pcd_open(struct inode *pinode, struct file *flip){
    return 0;
}
int pcd_release(struct inode *pinode, struct file *flip){
    return 0;
}


struct file_operations pcd_fops = 
{
    .open = pcd_open,
    .release = pcd_release,
    .write = pcd_write,
    .read = pcd_read,
    .llseek = pcd_lseek,
    .owner = THIS_MODULE

};

struct class *class_pcd;
struct device *device_pcd;
static int __init pcd_driver_init(void)
{

    alloc_chrdev_region(&device_num, 0, 7, "pcd_driver");
    pr_info(" driver <MAJOR> : <MINOR> %d : %d",MAJOR(device_num),MINOR(device_num));
    cdev_init(&pcd_cdev,&pcd_fops);
    pcd_cdev.owner =  THIS_MODULE;    

    cdev_add(&pcd_cdev,device_num,1);
    class_pcd = class_create(THIS_MODULE,"pcd_class");
    device_pcd = device_create(class_pcd,NULL,1,NULL,"pcd");

    pr_info("init driver success");

    return 0;
}

static void __exit pcd_driver_exit(void)
{
    device_destroy(class_pcd,device_num);
    class_destroy(class_pcd);
    cdev_del(&pcd_cdev);
    unregister_chedev_region(device_num,1);
    pr_info("module exited success");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sawan");
MODULE_DESCRIPTION("A pseudo driver for testing ");