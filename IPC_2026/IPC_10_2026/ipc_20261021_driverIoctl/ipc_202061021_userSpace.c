#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IPC_IOCTL_RESET \
    _IO('I', 1)

#define IPC_IOCTL_SET_VALUE \
    _IOW('I', 2, int)

#define IPC_IOCTL_GET_VALUE \
    _IOR('I', 3, int)

int main(void)
{
    int fd = open(
        "/dev/ipc_driver_ioctl",
        O_RDWR
    );

    int value = 1234;

    ioctl(
        fd,
        IPC_IOCTL_SET_VALUE,
        &value
    );

    value = 0;

    ioctl(
        fd,
        IPC_IOCTL_GET_VALUE,
        &value
    );

    printf(
        "Driver value = %d\n",
        value
    );

    ioctl(
        fd,
        IPC_IOCTL_RESET
    );

    close(fd);

    return 0;
}




