#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>

int main(void)
{
    int fd = open(
        "/dev/ipc_driver_epoll",
        O_RDWR
    );

    int epfd = epoll_create1(0);

    struct epoll_event event;

    event.events = EPOLLIN;
    event.data.fd = fd;

    epoll_ctl(
        epfd,
        EPOLL_CTL_ADD,
        fd,
        &event
    );

    printf("Waiting for driver using epoll...\n");

    struct epoll_event events[1];

    int n = epoll_wait(
        epfd,
        events,
        1,
        -1
    );

    if (n > 0)
        printf("Driver event received\n");

    close(epfd);
    close(fd);

    return 0;
}




