// ipc_20261021_driverPipe.c

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/mutex.h>

#define DEVICE_NAME "ipc_driver_pipe"
#define BUFFER_SIZE 1024

static char buffer[BUFFER_SIZE];
static size_t data_size;

static DEFINE_MUTEX(buffer_mutex);
static DECLARE_WAIT_QUEUE_HEAD(read_queue);

static ssize_t driver_read(
    struct file* file,
    char __user* user_buffer,
    size_t count,
    loff_t* offset)
{
    int ret;

    ret = wait_event_interruptible(
        read_queue,
        data_size > 0);

    if (ret)
        return ret;

    mutex_lock(&buffer_mutex);

    if (count > data_size)
        count = data_size;

    if (copy_to_user(
        user_buffer,
        buffer,
        count))
    {
        mutex_unlock(&buffer_mutex);
        return -EFAULT;
    }

    data_size = 0;

    mutex_unlock(&buffer_mutex);

    return count;
}

static ssize_t driver_write(
    struct file* file,
    const char __user* user_buffer,
    size_t count,
    loff_t* offset)
{
    if (count > BUFFER_SIZE)
        count = BUFFER_SIZE;

    mutex_lock(&buffer_mutex);

    if (copy_from_user(
        buffer,
        user_buffer,
        count))
    {
        mutex_unlock(&buffer_mutex);
        return -EFAULT;
    }

    data_size = count;

    mutex_unlock(&buffer_mutex);

    wake_up_interruptible(&read_queue);

    return count;
}

static const struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read = driver_read,
    .write = driver_write,
};

static struct miscdevice device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
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



