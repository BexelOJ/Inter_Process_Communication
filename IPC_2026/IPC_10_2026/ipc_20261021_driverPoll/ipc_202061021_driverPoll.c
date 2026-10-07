// ipc_20261021_driverPoll.c

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/uaccess.h>

static char data[256];
static int data_ready;

static DECLARE_WAIT_QUEUE_HEAD(wait_queue);

static ssize_t driver_read(
    struct file* file,
    char __user* buffer,
    size_t count,
    loff_t* offset)
{
    if (!data_ready)
        return 0;

    if (count > sizeof(data))
        count = sizeof(data);

    if (copy_to_user(buffer, data, count))
        return -EFAULT;

    data_ready = 0;

    return count;
}

static ssize_t driver_write(
    struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* offset)
{
    if (count > sizeof(data))
        count = sizeof(data);

    if (copy_from_user(data, buffer, count))
        return -EFAULT;

    data_ready = 1;

    wake_up_interruptible(&wait_queue);

    return count;
}

static __poll_t driver_poll(
    struct file* file,
    poll_table* wait)
{
    __poll_t mask = 0;

    poll_wait(
        file,
        &wait_queue,
        wait);

    if (data_ready)
        mask |= EPOLLIN | EPOLLRDNORM;

    return mask;
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
    .name = "ipc_driver_poll",
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



