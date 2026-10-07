// ipc_20261021_driverIoctl.c

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#define IPC_IOCTL_RESET \
    _IO('I', 1)

#define IPC_IOCTL_SET_VALUE \
    _IOW('I', 2, int)

#define IPC_IOCTL_GET_VALUE \
    _IOR('I', 3, int)

static int driver_value;

static long driver_ioctl(
    struct file* file,
    unsigned int cmd,
    unsigned long arg)
{
    int value;

    switch (cmd)
    {
    case IPC_IOCTL_RESET:

        driver_value = 0;

        pr_info("driver: reset\n");

        break;

    case IPC_IOCTL_SET_VALUE:

        if (copy_from_user(
            &value,
            (int __user*)arg,
            sizeof(value)))
        {
            return -EFAULT;
        }

        driver_value = value;

        pr_info(
            "driver: value = %d\n",
            driver_value
        );

        break;

    case IPC_IOCTL_GET_VALUE:

        value = driver_value;

        if (copy_to_user(
            (int __user*)arg,
            &value,
            sizeof(value)))
        {
            return -EFAULT;
        }

        break;

    default:
        return -ENOTTY;
    }

    return 0;
}

static const struct file_operations fops =
{
    .owner = THIS_MODULE,
    .unlocked_ioctl = driver_ioctl,
};

static struct miscdevice device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "ipc_driver_ioctl",
    .fops = &fops,
};

static int __init driver_init(void)
{
    return misc_register(&device);
}

static void __exit driver_exit(void)
{
    misc_deregister(&device);
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");



