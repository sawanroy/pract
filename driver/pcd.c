#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#define DEV_MEM_SIZE 512

#undef pr_fmt
#define pr_fmt(fmt) "%s : "fmt,__func__

char device_buf[DEV_MEM_SIZE];


dev_t device_num;

struct cdev pcd_cdev;

loff_t pcd_lseek(struct file *flip, loff_t offset, int whence){

    pr_info("llseek function called");
    pr_info("curernt file position = %lld",flip->f_pos);
    switch (whence)
    {
    case SEEK_SET:
        if((offset > DEV_MEM_SIZE) || (offset < 0))
            return -ENIVAL;
        flip->f_pos = offset;
        break;
    case SEEK_CUR:
        temp = flip->f_pos + offset;
        if((temp > DEV_MEM_SIZE) || (temp < 0))
            return _ENIVAL
        flip->f_pos += offset;
        break;
    case SEEK_END:
        temp = DEV_MEM_SIZE + offset;
        if((temp > DEV_MEM_SIZE)||(temp < 0))
            return -ENIVAL;
        flip->f_pos = DEV_MEM_SIZE+offset;
        break;    
    default:
        pr_info("pcd_llseek error ");
        retrun -EINVAL;
        break;
    }
    pr_info("updated file position = %lld",flip->f_pos);
    return 0;
}
ssize_t pcd_read(struct file *flip, char __user *buff, size_t count, loff_t *f_pos){

    pr_info("read requerst for the %zu bytes \n",count);
    pr_info("Before reading current file position = %lld",*f_pos);
    /* Adust the count*/
    if((count +*f_pos)>DEV_MEM_SIZE)
    {
        count = DEV_MEM_SIZE-*f_pos;
    }

    /*copy to user */
    if(copy_to_user(buff,&device_buf[*f_pos],count)){
        return -EFAULT;
    }
    /*update the current pointer position*/
    *f_pos +=count;
    pr_info("NUmber of bytes successfully read = %zu \n",count);
    pr_info("Updated file position = %lld",*f_pos);
    return 0;
}
ssize_t pcd_write(struct file *flip, const char __user *buff, size_t count, loff_t *f_pos){

    pr_info("Write requerst for the %zu bytes \n",count);
    pr_info("Before Writing current file position = %lld",*f_pos);
    /* Adust the count*/
    if((count +*f_pos)>DEV_MEM_SIZE)
    {
        count = DEV_MEM_SIZE-*f_pos;
    }
    if(!count)
    {
        return -ENOMEM;
    }

    /*copy from user */
    if(copy_from_user(&device_buf[*f_pos],buff,count)){
        return -EFAULT;
    }
    /*update the current pointer position*/
    *f_pos +=count;
    pr_info("NUmber of bytes successfully write = %zu \n",count);
    pr_info("Updated file position = %lld",*f_pos);
    return 0;
}
int pcd_open(struct inode *pinode, struct file *flip){
    pr_info("pcd driver open success")
    return 0;
}
int pcd_release(struct inode *pinode, struct file *flip){
    pr_info("pcd driver release success")
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