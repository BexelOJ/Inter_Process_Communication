#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

int main(void)
{
    int epollFd;
    int pipeFd[2];

    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];

    char buffer[5];

    if (pipe(pipeFd) < 0)
    {
        perror("pipe");
        return EXIT_FAILURE;
    }

    epollFd = epoll_create1(0);

    if (epollFd < 0)
    {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    /*
     * No EPOLLET.
     *
     * Therefore this is Level Triggered.
     */
    event.events = EPOLLIN;
    event.data.fd = pipeFd[0];

    epoll_ctl(epollFd,
        EPOLL_CTL_ADD,
        pipeFd[0],
        &event);

    write(pipeFd[1],
        "ABCDEFGHIJ",
        10);

    for (int i = 0; i < 3; i++)
    {
        int count = epoll_wait(epollFd,
            events,
            MAX_EVENTS,
            1000);

        printf("\nepoll_wait() returned %d\n", count);

        if (count > 0)
        {
            int bytes = read(pipeFd[0],
                buffer,
                sizeof(buffer));

            printf("Read %d bytes\n", bytes);
        }
    }

    close(pipeFd[0]);
    close(pipeFd[1]);
    close(epollFd);

    return EXIT_SUCCESS;
}



