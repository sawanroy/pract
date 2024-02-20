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
#define RDONLY 0x1
#define WRONLY 0x10
#define RDWR 0x11
struct pcd_drv_private_data pcdrv_data =
{
    .total_devices = NO_OF_DEVICES,
    .pcdev_data = {
        [0] = {
            .buffer = device_buf_pcdev1,
            .size = MEM_SIZE_PCDEV1,
            .serial_num = "pcdev_1",
            .perm = RDONLY, /* RDONLY */
        },
        [1] = {
            .buffer = device_buf_pcdev2,
            .size = MEM_SIZE_PCDEV2,
            .serial_num = "pcdev_1",
            .perm = WRONLY, /* WRONLY */
        },
        [2] = {
            .buffer = device_buf_pcdev3,
            .size = MEM_SIZE_PCDEV3,
            .serial_num = "pcdev_1",
            .perm = RDWR, /* RDWR */
        },        
        [3] = {
            .buffer = device_buf_pcdev4,
            .size = MEM_SIZE_PCDEV4,
            .serial_num = "pcdev_1",
            .perm = RDWR, /* RDWR */
        }

    }
};


loff_t pcd_lseek(struct file *flip, loff_t offset, int whence){
     loff_t temp;

    struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)flip->private_data;
    int max_size = pcdev_data->size;

    pr_info("llseek function called");
    pr_info("curernt file position = %lld",flip->f_pos);
    switch (whence)
    {
    case SEEK_SET:
        if((offset > max_size) || (offset < 0))
            return -EINVAL; 
        flip->f_pos = offset;
        break;
    case SEEK_CUR:
        temp = flip->f_pos + offset;
        if((temp > max_size) || (temp < 0))
            return -EINVAL;
        flip->f_pos += offset;
        break;
    case SEEK_END:
        temp = max_size + offset;
        if((temp > max_size)||(temp < 0))
            return -EINVAL;
        flip->f_pos = max_size+offset;
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
  
    struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)flip->private_data;
    int max_size = pcdev_data->size;
    pr_info("read requerst for the %zu bytes \n",count);
    pr_info("Before reading current file position = %lld",*f_pos);
    /* Adust the count*/
    if((count +*f_pos)>max_size)
    {
        count = max_size - *f_pos;
    }

    /*copy to user */
    if(copy_to_user(buff,pcdev_data->buffer+(*f_pos),count)){
        return -EFAULT;
    }
    /*update the current pointer position*/
    *f_pos +=count;
    pr_info("NUmber of bytes successfully read = %zu \n",count);
    pr_info("Updated file position = %lld",*f_pos);
    
    return count;
}
ssize_t pcd_write(struct file *flip, const char __user *buff, size_t count, loff_t *f_pos){

    struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)flip->private_data;
    int max_size = pcdev_data->size;

    pr_info("Write requerst for the %zu bytes \n",count);
    pr_info("Before Writing current file position = %lld",*f_pos);
    /* Adust the count*/
    if((count +*f_pos)>max_size)
    {
        count = max_size - *f_pos;
    }
    if(!count)
    {
        return -ENOMEM;
    }

    /*copy from user */
    if(copy_from_user(pcdev_data->buffer+(*f_pos),buff,count)){
        return -EFAULT;
    }
    /*update the current pointer position*/
    *f_pos +=count;
    pr_info("NUmber of bytes successfully write = %zu \n",count);
    pr_info("Updated file position = %lld",*f_pos);
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
    pr_info("pcd driver open success");

    int ret;
    int minor_n;
    struct pcdev_private_data *pcdev_data;

    minor_n = MINOR(pinode->i_rdev);
    pr_info("Minor acess of %d",minor_n);

    /*To get the information of private data structure*/
    pcdev_data = container_of(pinode->i_cdev,struct pcdev_private_data,cdev);
    /* to supply the private data strcutre to ohther function can use*/
    flip->private_data = pcdev_data;

    ret = check_permission(pcdev_data->perm,flip->f_mode);
        (!ret)?pr_info("open was successfull\n"):pr_info("open was unsuccessfull\n");
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


static int __init pcd_driver_init(void)
{
    int ret;
    int i;
    ret = alloc_chrdev_region(&pcdrv_data.device_num, 0, NO_OF_DEVICES, "pcd_driver");
    if(ret < 0)
    {   
        pr_err("error while alloc chrdev");
        goto out;
    }

    pcdrv_data.class_pcd = class_create("pcd_class");
    if(IS_ERR(pcdrv_data.class_pcd))
    {
        pr_err("Class creation failed");
        ret = PTR_ERR(pcdrv_data.class_pcd);
        goto chrdev_unregister;
    }

    for(i=0;i<NO_OF_DEVICES;i++){
    pr_info(" driver <MAJOR> : <MINOR> %d : %d",MAJOR(pcdrv_data.device_num +i),MINOR(pcdrv_data.device_num+i));
    
    /*cdev init */
    cdev_init(&pcdrv_data.pcdev_data[i].cdev,&pcd_fops);
    pcdrv_data.pcdev_data[i].cdev.owner =  THIS_MODULE;    
    ret = cdev_add(&pcdrv_data.pcdev_data[i].cdev,pcdrv_data.device_num,1);
    if(ret < 0)
    {
        pr_err("error while cdev add");
        goto cdev_del;
    }

    pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd,NULL,pcdrv_data.device_num,NULL,"pcd-%d",i);
    if(IS_ERR(pcdrv_data.device_pcd))
    {
        pr_err("device creation failed");
        ret = PTR_ERR(pcdrv_data.device_pcd);
        goto class_distroy;
    }
    }
    pr_info("init driver success");

    return 0;
cdev_del:
class_distroy:
    for(;i<NO_OF_DEVICES;i++)
    {
     device_destroy(pcdrv_data.class_pcd,pcdrv_data.device_num+i);
     cdev_del(&pcdrv_data.pcdev_data[i].cdev);
    }
    class_destroy(pcdrv_data.class_pcd); 
chrdev_unregister:
    unregister_chrdev_region(pcdrv_data.device_num,NO_OF_DEVICES);
out:
    return ret;

}

static void __exit pcd_driver_exit(void)
{
    #if 0
    device_destroy(class_pcd,device_num);
    class_destroy(class_pcd);
    cdev_del(&pcdev_data);
    unregister_chrdev_region(device_num,1);

    pr_info("module exited success");
    #endif
}

module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sawan");
MODULE_DESCRIPTION("A pseudo driver for testing ");
