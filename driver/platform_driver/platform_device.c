#include<linux/module.h>
#include<linux/platform_device.h>

//1. create 2. platfrom device

struct platform_device platform_pcdev_1 = {
    .name = "pseudo-char-device",
    .id = 0
};

struct platform_device platform_pcdev_2 = {
    .name = "pseudo-char-device",
    .id = 1
};


static int __init pdev_platform_init(void)
{
    //register platfrom device

    platform_device_register(&platform_pcdev_1);
    platform_device_register(&platform_pcdev_2);
    return 0;
}

static void __exit pdev_platform_exit(void)
{
    //unregister platfrom devices

    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);
}

module_init(pdev_platform_init);
module_exit(pdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sawan");
MODULE_DESCRIPTION("A pseudo platform driver");
