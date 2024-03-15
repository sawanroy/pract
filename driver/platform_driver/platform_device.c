#include<linux/module.h>

//1. create 2. platfrom device

struct platform_device platform_pcdev_1 = {
    .name = "pseudo-char-device",
    .id = 0
}

struct platform_device platform_pcdev_2 = {
    .name = "pseudo-char-device",
    .id = 1
}


static int __init pdev_platform_init(void)
{
    //register platfrom device

    platform_device_register(&platform_pcdev_1);
    platform_device_register(&platform_pcdev_2);
    return 0;
}

static void __exit platform_exit(void)
{
    //unregister platfrom devices

    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);
}