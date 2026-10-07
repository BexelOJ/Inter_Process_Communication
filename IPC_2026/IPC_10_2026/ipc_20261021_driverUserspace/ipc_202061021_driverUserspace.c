// ipc_20261021_driverUserspace.c

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "ipc_userspace"

static char kernel_buffer[256];

static ssize_t driver_read(
    struct file* file,
    char __user* buffer,
    size_t count,
    loff_t* offset)
{
    size_t len;

    len = strlen(kernel_buffer);

    if (*offset >= len)
        return 0;

    if (count > len - *offset)
        count = len - *offset;

    if (copy_to_user(
        buffer,
        kernel_buffer + *offset,
        count))
    {
        return -EFAULT;
    }

    *offset += count;

    return count;
}

static ssize_t driver_write(
    struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* offset)
{
    if (count >= sizeof(kernel_buffer))
        count = sizeof(kernel_buffer) - 1;

    if (copy_from_user(
        kernel_buffer,
        buffer,
        count))
    {
        return -EFAULT;
    }

    kernel_buffer[count] = '\0';

    pr_info(
        "ipc_userspace: received: %s\n",
        kernel_buffer);

    return count;
}

static const struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read = driver_read,
    .write = driver_write,
};

static struct miscdevice ipc_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &fops,
};

static int __init driver_init(void)
{
    int ret;

    ret = misc_register(&ipc_device);

    if (ret)
        return ret;

    pr_info("ipc_userspace: loaded\n");

    return 0;
}

static void __exit driver_exit(void)
{
    misc_deregister(&ipc_device);

    pr_info("ipc_userspace: unloaded\n");
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Userspace to kernel driver IPC");


