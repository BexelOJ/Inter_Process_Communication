// ipc_20261021_driverEventfd.c

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/eventfd.h>

#define IPC_EVENTFD_SET _IOW('E', 1, int)

static struct eventfd_ctx* event_ctx;

static long driver_ioctl(
    struct file* file,
    unsigned int cmd,
    unsigned long arg)
{
    int fd;

    switch (cmd)
    {
    case IPC_EVENTFD_SET:

        if (copy_from_user(
            &fd,
            (int __user*)arg,
            sizeof(fd)))
        {
            return -EFAULT;
        }

        if (event_ctx)
            eventfd_ctx_put(event_ctx);

        event_ctx = eventfd_ctx_fdget(fd);

        if (IS_ERR(event_ctx))
        {
            event_ctx = NULL;
            return -EINVAL;
        }

        pr_info("eventfd connected\n");

        return 0;

    default:
        return -EINVAL;
    }
}

static ssize_t driver_write(
    struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* offset)
{
    if (event_ctx)
        eventfd_signal(event_ctx, 1);

    return count;
}

static const struct file_operations fops =
{
    .owner = THIS_MODULE,
    .unlocked_ioctl = driver_ioctl,
    .write = driver_write,
};

static struct miscdevice device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "ipc_driver_eventfd",
    .fops = &fops,
};

static int __init driver_init(void)
{
    return misc_register(&device);
}

static void __exit driver_exit(void)
{
    if (event_ctx)
        eventfd_ctx_put(event_ctx);

    misc_deregister(&device);
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");



