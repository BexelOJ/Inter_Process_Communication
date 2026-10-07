// ipc_20261021_driverEpoll.c

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/uaccess.h>

static int event_ready;

static DECLARE_WAIT_QUEUE_HEAD(event_queue);

static ssize_t driver_write(
    struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* offset)
{
    event_ready = 1;

    wake_up_interruptible(&event_queue);

    return count;
}

static ssize_t driver_read(
    struct file* file,
    char __user* buffer,
    size_t count,
    loff_t* offset)
{
    event_ready = 0;

    return 0;
}

static __poll_t driver_poll(
    struct file* file,
    poll_table* wait)
{
    poll_wait(
        file,
        &event_queue,
        wait);

    if (event_ready)
        return EPOLLIN;

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
    .name = "ipc_driver_epoll",
    .fops = &fops,
};

static int __init driver_init(void)
{
    pr_info("driver_epoll loaded\n");

    return misc_register(&device);
}

static void __exit driver_exit(void)
{
    misc_deregister(&device);

    pr_info("driver_epoll unloaded\n");
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");



