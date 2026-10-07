#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

int main(void)
{
    int fd = open("/dev/ipc_driver_poll", O_RDWR);

    struct pollfd pfd =
    {
        .fd = fd,
        .events = POLLIN
    };

    printf("Waiting for driver event...\n");

    int ret = poll(&pfd, 1, -1);

    if (ret > 0 && (pfd.revents & POLLIN))
    {
        char buffer[256];

        int n = read(fd, buffer, sizeof(buffer));

        printf("Driver produced %d bytes\n", n);
    }

    close(fd);

    return 0;
}




