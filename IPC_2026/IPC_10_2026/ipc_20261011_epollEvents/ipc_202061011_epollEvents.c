#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

void printEvents(uint32_t events)
{
    if (events & EPOLLIN)
        printf("EPOLLIN\n");

    if (events & EPOLLOUT)
        printf("EPOLLOUT\n");

    if (events & EPOLLERR)
        printf("EPOLLERR\n");

    if (events & EPOLLHUP)
        printf("EPOLLHUP\n");

    if (events & EPOLLRDHUP)
        printf("EPOLLRDHUP\n");

    if (events & EPOLLET)
        printf("EPOLLET\n");

    if (events & EPOLLONESHOT)
        printf("EPOLLONESHOT\n");
}

int main(void)
{
    int epollFd;
    int pipeFd[2];

    struct epoll_event event;
    struct epoll_event events[10];

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

    event.events = EPOLLIN |
        EPOLLRDHUP |
        EPOLLERR |
        EPOLLHUP;

    event.data.fd = pipeFd[0];

    if (epoll_ctl(epollFd,
        EPOLL_CTL_ADD,
        pipeFd[0],
        &event) < 0)
    {
        perror("epoll_ctl");
        return EXIT_FAILURE;
    }

    write(pipeFd[1], "event\n", 6);

    int count = epoll_wait(epollFd,
        events,
        10,
        1000);

    if (count < 0)
    {
        perror("epoll_wait");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < count; i++)
    {
        printf("FD: %d\n", events[i].data.fd);
        printf("Events:\n");

        printEvents(events[i].events);
    }

    close(pipeFd[0]);
    close(pipeFd[1]);
    close(epollFd);

    return EXIT_SUCCESS;
}



