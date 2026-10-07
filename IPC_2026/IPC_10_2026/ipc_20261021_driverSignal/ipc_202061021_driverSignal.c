// ipc_20261021_driverSignal.c

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

static struct fasync_struct* async_queue;

static int driver_fasync(
    int fd,
    struct file* file,
    int on)
{
    return fasync_helper(
        fd,
        file,
        on,
        &async_queue
    );
}

static ssize_t driver_write(
    struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* offset)
{
    pr_info(
        "driver_signal: event occurred\n"
    );

    if (async_queue)
    {
        kill_fasync(
            &async_queue,
            SIGIO,
            POLL_IN
        );
    }

    return count;
}

static int driver_release(
    struct inode* inode,
    struct file* file)
{
    driver_fasync(
        -1,
        file,
        0
    );

    return 0;
}

static const struct file_operations fops =
{
    .owner = THIS_MODULE,
    .write = driver_write,
    .fasync = driver_fasync,
    .release = driver_release,
};

static struct miscdevice device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "ipc_driver_signal",
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



