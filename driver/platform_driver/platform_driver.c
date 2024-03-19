
#include"platform.h"
#include<linux/version.h>

struct pcd_drv_private_data pcdrv_data =
{
    // .total_devices = NO_OF_DEVICES,
    // .pcdev_data = {
    //     [0] = {
    //         .buffer = device_buf_pcdev1,
    //         .size = MEM_SIZE_PCDEV1,
    //         .serial_num = "pcdev_1",
    //         .perm = RDONLY, /* RDONLY */
    //     },
    //     [1] = {
    //         .buffer = device_buf_pcdev2,
    //         .size = MEM_SIZE_PCDEV2,
    //         .serial_num = "pcdev_2",
    //         .perm = WRONLY, /* WRONLY */
    //     },
    //     [2] = {
    //         .buffer = device_buf_pcdev3,
    //         .size = MEM_SIZE_PCDEV3,
    //         .serial_num = "pcdev_3",
    //         .perm = RDWR, /* RDWR */
    //     },        
    //     [3] = {
    //         .buffer = device_buf_pcdev4,
    //         .size = MEM_SIZE_PCDEV4,
    //         .serial_num = "pcdev_4",
    //         .perm = RDWR, /* RDWR */
    //     }

    // }
};


loff_t pcd_lseek(struct file *flip, loff_t offset, int whence){
    //  loff_t temp;

    // struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)flip->private_data;
    // int max_size = pcdev_data->size;

    // pr_info("llseek function called");
    // pr_info("curernt file position = %lld",flip->f_pos);
    // switch (whence)
    // {
    // case SEEK_SET:
    //     if((offset > max_size) || (offset < 0))
    //         return -EINVAL; 
    //     flip->f_pos = offset;
    //     break;
    // case SEEK_CUR:
    //     temp = flip->f_pos + offset;
    //     if((temp > max_size) || (temp < 0))
    //         return -EINVAL;
    //     flip->f_pos += offset;
    //     break;
    // case SEEK_END:
    //     temp = max_size + offset;
    //     if((temp > max_size)||(temp < 0))
    //         return -EINVAL;
    //     flip->f_pos = max_size+offset;
    //     break;    
    // default:
    //     pr_info("pcd_llseek error ");
    //     return -EINVAL;
    //     break;
    // }
    // pr_info("updated file position = %lld",flip->f_pos);
     return 0;
}
ssize_t pcd_read(struct file *flip, char __user *buff, size_t count, loff_t *f_pos){
  
    // struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)flip->private_data;
    // int max_size = pcdev_data->size;
    // pr_info("read requerst for the %zu bytes \n",count);
    // pr_info("Before reading current file position = %lld",*f_pos);
    // /* Adust the count*/
    // if((count +*f_pos)>max_size)
    // {
    //     count = max_size - *f_pos;
    // }

    // /*copy to user */
    // if(copy_to_user(buff,pcdev_data->buffer+(*f_pos),count)){
    //     return -EFAULT;
    // }
    // /*update the current pointer position*/
    // *f_pos +=count;
    // pr_info("NUmber of bytes successfully read = %zu \n",count);
    // pr_info("Updated file position = %lld",*f_pos);
    
    return count;
}
ssize_t pcd_write(struct file *flip, const char __user *buff, size_t count, loff_t *f_pos){

    // struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)flip->private_data;
    // int max_size = pcdev_data->size;

    // pr_info("Write requerst for the %zu bytes \n",count);
    // pr_info("Before Writing current file position = %lld",*f_pos);
    // /* Adust the count*/
    // if((count +*f_pos)>max_size)
    // {
    //     count = max_size - *f_pos;
    // }
    // if(!count)
    // {
    //     return -ENOMEM;
    // }

    // /*copy from user */
    // if(copy_from_user(pcdev_data->buffer+(*f_pos),buff,count)){
    //     return -EFAULT;
    // }
    // /*update the current pointer position*/
    // *f_pos +=count;
    // pr_info("NUmber of bytes successfully write = %zu \n",count);
    // pr_info("Updated file position = %lld",*f_pos);
    return count;
}
int check_permission(int dev_perm, int acc_mode)
{
    if(dev_perm == RDWR)
        return 0;
    /*it ensure read only permission */    
    if((dev_perm == RDONLY) && ((acc_mode & FMODE_READ)&&!(acc_mode & FMODE_WRITE))) 
        return 0;
    /* it ensures write only permission */    
    if((dev_perm == WRONLY) && ((acc_mode & FMODE_WRITE)&&!(acc_mode & FMODE_READ))) 
        return 0;     

    return -EPERM;           
}


int pcd_open(struct inode *pinode, struct file *flip){

    int ret =0;
    // int minor_n;
    // struct pcdev_private_data *pcdev_data;
    // pr_info("pcd driver open success");
    // minor_n = MINOR(pinode->i_rdev);
    // pr_info("Minor acess of %d",minor_n);

    // /*To get the information of private data structure*/
    // pcdev_data = container_of(pinode->i_cdev,struct pcdev_private_data,cdev);
    // /* to supply the private data strcutre to ohther function can use*/
    // flip->private_data = pcdev_data;

    // ret = check_permission(pcdev_data->perm,flip->f_mode);
    //     (!ret)?pr_info("open was successfull\n"):pr_info("open was unsuccessfull\n");
    return ret;
}
int pcd_release(struct inode *pinode, struct file *flip){
    pr_info("pcd driver release success");
    return 0;
}

int pcd_platfrom_driver_remove(struct platform_device *pdev)
{

    pr_info("device is removed");
    return 0;
}


int pcd_platfrom_driver_probe(struct platform_device *pdev)
{
    pr_info("device is detected");
    return 0;
}

struct platform_driver pcd_platform_dev = 
{
    .probe = pcd_platfrom_driver_probe,
    .remove = pcd_platfrom_driver_remove,
    .driver = {
        .name = "pseudo-char-device"
    }
};


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
    /*1. Dynamically allocated a device number for MAX_DEVICES*/
    ret = alloc_chrdev_region(&pcdrv_data.device_num_base, 0, NO_OF_DEVICES, "pcd_driver");
    if(ret < 0)
        {   
            pr_err("error while alloc chrdev");
            return ret;
        }

    /*2 . Create device class under /sys/class*/
    #if LINUX_VERSION_CODE <= KERNEL_VERSION(5,0,0)
        pcdrv_data.class_pcd = class_create(THIS_MODULE,"pcd_drv_class");
    #else
        pcdrv_data.class_pcd = class_create("pcd_drv_class");
    #endif
    if(IS_ERR(pcdrv_data.class_pcd))
    {
        pr_err("Class creation failed");
        ret = PTR_ERR(pcdrv_data.class_pcd);
        unregister_chrdev_region(pcdrv_data.device_num_base,NO_OF_DEVICES);
        return ret;
    }


    platform_driver_register(&pcd_platform_dev);
    return 0;

}

static void __exit pcd_driver_exit(void)
{
    /* Unregister the platform driver*/
   platform_driver_unregister(&pcd_platform_dev);
   /*Class destroy */
   class_destroy(pcdrv_data.class_pcd);
    /*Unregister device number for MAX_DEVICES*/
    unregister_chrdev_region(pcdrv_data.device_num_base,NO_OF_DEVICES);
    pr_info("driver is unloaded");

}

module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sawan");
MODULE_DESCRIPTION("A pseudo driver for testing ");
