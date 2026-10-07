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

    char buffer[100];

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

    event.events = EPOLLIN;
    event.data.fd = pipeFd[0];

    if (epoll_ctl(epollFd,
        EPOLL_CTL_ADD,
        pipeFd[0],
        &event) < 0)
    {
        perror("epoll_ctl");
        return EXIT_FAILURE;
    }

    printf("Waiting for an event...\n");

    write(pipeFd[1], "Hello epoll\n", 12);

    int count = epoll_wait(epollFd,
        events,
        MAX_EVENTS,
        -1);

    if (count < 0)
    {
        perror("epoll_wait");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < count; i++)
    {
        if (events[i].events & EPOLLIN)
        {
            int bytes = read(events[i].data.fd,
                buffer,
                sizeof(buffer) - 1);

            buffer[bytes] = '\0';

            printf("Event received: %s", buffer);
        }
    }

    close(pipeFd[0]);
    close(pipeFd[1]);
    close(epollFd);

    return EXIT_SUCCESS;
}



