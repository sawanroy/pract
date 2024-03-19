#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include <linux/errno.h>
#include<linux/platform_device.h>

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

struct pcdev_platform_data
{
    int size;
    int perm;
    const char *serial_number;
};

/* defination of private device data strucure*/
struct pcdev_private_data
{
    struct pcdev_platform_data pdrv_data;
    char *buffer;
    dev_t dev_num;
    // unsigned size;
    // const char * serial_num;
    // //int perm;
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
#define RDONLY 0x1
#define WRONLY 0x10
#define RDWR 0x11