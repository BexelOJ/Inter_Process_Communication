#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/ioctl.h>
#include <stdint.h>

#define IPC_EVENTFD_SET _IOW('E', 1, int)

int main(void)
{
    int eventfd = eventfd(0, 0);

    int driver = open(
        "/dev/ipc_driver_eventfd",
        O_WRONLY
    );

    ioctl(
        driver,
        IPC_EVENTFD_SET,
        &eventfd
    );

    printf("Triggering driver...\n");

    write(driver, "X", 1);

    uint64_t value;

    read(
        eventfd,
        &value,
        sizeof(value)
    );

    printf(
        "eventfd notification received: %lu\n",
        value
    );

    close(driver);
    close(eventfd);

    return 0;
}



