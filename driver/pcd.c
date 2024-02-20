#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include <linux/errno.h>

#define NO_OF_DEVICES 4
#define MEM_SIZE_PCDEV1 1024
#define MEM_SIZE_PCDEV2 512
#define MEM_SIZE_PCDEV3 1024
#define MEM_SIZE_PCDEV4 512

#undef pr_fmt
#define pr_fmt(fmt) "%s : "fmt,__func__

char device_buf_pcdev1[MEM_SIZE_PCDEV1];
char device_buf_pcdev2[MEM_SIZE_PCDEV2];
char device_buf_pcdev3[MEM_SIZE_PCDEV3];
char device_buf_pcdev4[MEM_SIZE_PCDEV4];
/* defination of private device data strucure*/
struct pcdev_private_data
{
    char *buffer;
    unsigned size;
    const char * serial_num;
    int perm;
    struct cdev cdev;
};
/* defination of private driver data strucure*/
struct pcd_drv_private_data
{
    int total_devices;
    dev_t device_num;
    struct class *class_pcd;
    struct device *device_pcd;
    struct pcdev_private_data pcdev_data[NO_OF_DEVICES];

};

struct pcd_drv_private_data pcdrv_data =
{
    .total_devices = NO_OF_DEVICES,
    .pcdev_data = {
        [0] = {
            .buffer = device_buf_pcdev1,
            .size = MEM_SIZE_PCDEV1,
            .serial_num = "pcdev_1",
            .perm = 0x1, /* RDONLY */
        },
        [1] = {
            .buffer = device_buf_pcdev2,
            .size = MEM_SIZE_PCDEV2,
            .serial_num = "pcdev_1",
            .perm = 0x1, /* RDONLY */
        },
        [2] = {
            .buffer = device_buf_pcdev3,
            .size = MEM_SIZE_PCDEV3,
            .serial_num = "pcdev_1",
            .perm = 0x1, /* RDONLY */
        },        
        [3] = {
            .buffer = device_buf_pcdev4,
            .size = MEM_SIZE_PCDEV4,
            .serial_num = "pcdev_1",
            .perm = 0x1, /* RDONLY */
        }

    }
};


loff_t pcd_lseek(struct file *flip, loff_t offset, int whence){
     loff_t temp;
    pr_info("llseek function called");
    pr_info("curernt file position = %lld",flip->f_pos);
    switch (whence)
    {
    case SEEK_SET:
        if((offset > DEV_MEM_SIZE) || (offset < 0))
            return -EINVAL; 
        flip->f_pos = offset;
        break;
    case SEEK_CUR:
        temp = flip->f_pos + offset;
        if((temp > DEV_MEM_SIZE) || (temp < 0))
            return -EINVAL;
        flip->f_pos += offset;
        break;
    case SEEK_END:
        temp = DEV_MEM_SIZE + offset;
        if((temp > DEV_MEM_SIZE)||(temp < 0))
            return -EINVAL;
        flip->f_pos = DEV_MEM_SIZE+offset;
        break;    
    default:
        pr_info("pcd_llseek error ");
        return -EINVAL;
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
    pr_info("pcd driver open success");
    return 0;
}
int pcd_release(struct inode *pinode, struct file *flip){
    pr_info("pcd driver release success");
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


static int __init pcd_driver_init(void)
{
    int ret;
    ret = alloc_chrdev_region(pcd_drv_private_data.d, 0, NO_OF_DEVICES, "pcd_driver");
    if(ret < 0)
    {   
        pr_err("error while alloc chrdev");
        goto out;
    }
    pr_info(" driver <MAJOR> : <MINOR> %d : %d",MAJOR(device_num),MINOR(device_num));
    cdev_init(&pcd_cdev,&pcd_fops);
    pcd_cdev.owner =  THIS_MODULE;    

    ret = cdev_add(&pcd_cdev,device_num,1);
    if(ret < 0)
    {
        pr_err("error while cdev add");
        goto chrdev_unregister;
    }
    class_pcd = class_create(THIS_MODULE,"pcd_class");
    if(IS_ERR(class_pcd))
    {
        pr_err("Class creation failed");
        ret = PTR_ERR(class_pcd);
        goto cdev_del;
    }
    device_pcd = device_create(class_pcd,NULL,1,NULL,"pcd");
    if(IS_ERR(device_pcd))
    {
        pr_err("device creation failed");
        ret = PTR_ERR(device_pcd);
        goto class_distroy;
    }

    pr_info("init driver success");

    return 0;
class_distroy:
    class_destroy(class_pcd);
cdev_del:
    cdev_del(&pcd_cdev);
chrdev_unregister:
    unregister_chrdev_region(device_num,1);
out:
    return ret;

}

static void __exit pcd_driver_exit(void)
{
    device_destroy(class_pcd,device_num);
    class_destroy(class_pcd);
    cdev_del(&pcd_cdev);
    unregister_chrdev_region(device_num,1);

    pr_info("module exited success");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sawan");
MODULE_DESCRIPTION("A pseudo driver for testing ");
