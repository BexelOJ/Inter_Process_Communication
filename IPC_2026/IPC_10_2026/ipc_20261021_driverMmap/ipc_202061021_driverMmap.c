// ipc_20261021_driverMmap.c

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>

#define BUFFER_SIZE PAGE_SIZE

static void* shared_buffer;

static int driver_mmap(
    struct file* file,
    struct vm_area_struct* vma)
{
    unsigned long size;

    size = vma->vm_end - vma->vm_start;

    if (size > BUFFER_SIZE)
        return -EINVAL;

    return remap_vmalloc_range(
        vma,
        shared_buffer,
        0
    );
}

static const struct file_operations fops =
{
    .owner = THIS_MODULE,
    .mmap = driver_mmap,
};

static struct miscdevice device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "ipc_driver_mmap",
    .fops = &fops,
};

static int __init driver_init(void)
{
    shared_buffer = vmalloc(BUFFER_SIZE);

    if (!shared_buffer)
        return -ENOMEM;

    memset(
        shared_buffer,
        0,
        BUFFER_SIZE
    );

    snprintf(
        shared_buffer,
        BUFFER_SIZE,
        "Hello from kernel mmap buffer\n"
    );

    return misc_register(&device);
}

static void __exit driver_exit(void)
{
    misc_deregister(&device);

    vfree(shared_buffer);
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");



