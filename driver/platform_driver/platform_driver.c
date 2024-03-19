
#include"platform.h"
#include<linux/slab.h>
#include<linux/version.h>


/* defination of private device data strucure*/
struct pcdev_private_data
{
    struct pcdev_platform_data pdata;
    char *buffer;
    dev_t dev_num;
    struct cdev cdev;
};
/* defination of private driver data strucure*/
struct pcd_drv_private_data
{
    int total_devices;
    dev_t device_num_base;
    struct class *class_pcd;
    struct device *device_pcd;
    //struct pcdev_private_data pcdev_data[NO_OF_DEVICES];

};
struct pcd_drv_private_data pcdrv_data;



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
struct file_operations pcd_fops = 
{
    .open = pcd_open,
    .release = pcd_release,
    .write = pcd_write,
    .read = pcd_read,
    .llseek = pcd_lseek,
    .owner = THIS_MODULE

};
int pcd_platfrom_driver_remove(struct platform_device *pdev)
{
    struct pcdev_private_data *dev_data = dev_get_drvdata(&pdev->dev);
    /*Remove a device that was created with device_create*/
    device_destroy(pcdrv_data.class_pcd,dev_data->dev_num);
    /*Remove a cdev entry from system*/
    /*Free the momery held by the device*/

    pcdrv_data.total_devices--;
    pr_info("device is removed");
    return 0;
}


int pcd_platfrom_driver_probe(struct platform_device *pdev)
{
    int ret;

    struct pcdev_platform_data *pdata;
    struct pcdev_private_data *dev_data;

    /*Get the platform data*/
    pdata = (struct pcdev_platform_data*)dev_get_platdata(&pdev->dev);
    if(!pdata){
        pr_info("No platform data is avaiable");
        ret = -EINVAL;
        goto out;
    }
    /* Dynamically allocating th ememory to device private data*/
    dev_data = kzalloc(sizeof(*dev_data),GFP_KERNEL);
    if(!dev_data)
    {
        pr_info("cannot allocate the memory");
        ret = -ENOMEM;
        goto out;
    }
    
    /*save the device private dtta in pdev structure*/
    dev_set_drvdata(&pdev->dev,dev_data);
    dev_data->pdata.size = pdata->size;
    dev_data->pdata.perm = pdata->perm;
    dev_data->pdata.serial_number = pdata->serial_number;

    pr_info("device size %d\n",dev_data->pdata.size);
    pr_info("device permission %d\n",dev_data->pdata.perm);
    pr_info("device serial number %s\n",dev_data->pdata.serial_number);

    /* Dynamically allcoat eth ememory for th edeivce buffer */
    dev_data->buffer = kzalloc(dev_data->pdata.size,GFP_KERNEL);
    if(!dev_data->buffer)
    {
        pr_info("cannot allocate the memory");
        ret = -ENOMEM;
        goto dev_data_free;
    }

    /* Get the device number*/
    dev_data->dev_num = pcdrv_data.device_num_base + pdev->id;

    /*Do cdev init and cdev add*/
    cdev_init(&dev_data->cdev,&pcd_fops);
    dev_data->cdev.owner =  THIS_MODULE;
    ret = cdev_add(&dev_data->cdev,dev_data->dev_num,1);
    if(ret < 0)
    {
        pr_err("error while cdev add");
        goto buffer_free;
    }

    /* Create device file for the dectected platform device*/
    pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd,NULL,dev_data->dev_num,NULL,"pcdev-%d",pdev->id);
    if(IS_ERR(pcdrv_data.device_pcd))
    {
        pr_err("device creation failed");
        ret = PTR_ERR(pcdrv_data.device_pcd);
        goto cdev_del;
    }

    pcdrv_data.total_devices++;
    pr_info("The probe was successful");
    return 0;

/*error handling*/    
cdev_del:
    cdev_del(&dev_data->cdev);

buffer_free:
    kfree(dev_data->buffer);    
dev_data_free:
    kfree(dev_data);
out:
    pr_info("device probe failed");
    return ret;
}
struct platform_driver pcd_platform_dev = 
{
    .probe = pcd_platfrom_driver_probe,
    .remove = pcd_platfrom_driver_remove,
    .driver = {
        .name = "pseudo-char-device"
    }
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
