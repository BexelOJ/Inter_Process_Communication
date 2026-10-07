#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>

int main(void)
{
    int fd = open(
        "/dev/ipc_driver_select",
        O_RDWR
    );

    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    printf("Waiting using select()...\n");

    int ret = select(
        fd + 1,
        &readfds,
        NULL,
        NULL,
        NULL
    );

    if (ret > 0 && FD_ISSET(fd, &readfds))
        printf("Driver became readable\n");

    close(fd);

    return 0;
}




