#include"platform.h"



void pcdev_release(struct device *dev)
{
    pr_info("device released");
}
//1. create 2 platform data

    struct pcdev_platform_data pcdrv_data[2] = {
        [0] = {.size = 512, .perm = RDWR, .serial_number = "pcdev1"},
        [1] = {.size = 1024, .perm = RDWR, .serial_number = "pcdev2"}
    };

//2. create 2 platform device

struct platform_device platform_pcdev_1 = {
    .name = "pseudo-char-device",
    .id = 0,
    .dev = {
        .platform_data = &pcdrv_data[0],
        .release = pcdev_release
    }
};

struct platform_device platform_pcdev_2 = {
    .name = "pseudo-char-device",
    .id = 1,
    .dev = {
        .platform_data = &pcdrv_data[1],
        .release = pcdev_release
    }
};


static int __init pdev_platform_init(void)
{
    //register platfrom device

    platform_device_register(&platform_pcdev_1);
    platform_device_register(&platform_pcdev_2);

    pr_info("device setup is loaded");
    return 0;
}

static void __exit pdev_platform_exit(void)
{
    //unregister platfrom devices

    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);
    pr_info("device setup is uploaded");
}

module_init(pdev_platform_init);
module_exit(pdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sawan");
MODULE_DESCRIPTION("A pseudo platform driver");
