// ipc_20261021_driverSelect.c

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/uaccess.h>

static int ready;

static DECLARE_WAIT_QUEUE_HEAD(wait_queue);

static ssize_t driver_write(
    struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* offset)
{
    ready = 1;

    wake_up_interruptible(&wait_queue);

    return count;
}

static ssize_t driver_read(
    struct file* file,
    char __user* buffer,
    size_t count,
    loff_t* offset)
{
    ready = 0;

    return 0;
}

static __poll_t driver_poll(
    struct file* file,
    poll_table* wait)
{
    poll_wait(
        file,
        &wait_queue,
        wait);

    if (ready)
        return EPOLLIN | EPOLLRDNORM;

    return 0;
}

static const struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read = driver_read,
    .write = driver_write,
    .poll = driver_poll,
};

static struct miscdevice device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "ipc_driver_select",
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



