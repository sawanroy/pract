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


#define RDONLY 0x1
#define WRONLY 0x10
#define RDWR 0x11