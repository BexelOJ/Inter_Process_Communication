#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

int main(void)
{
    int eventFd;
    int epollFd;

    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];

    eventFd = eventfd(0, 0);

    if (eventFd < 0)
    {
        perror("eventfd");
        return EXIT_FAILURE;
    }

    epollFd = epoll_create1(0);

    if (epollFd < 0)
    {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    event.events = EPOLLIN;
    event.data.fd = eventFd;

    epoll_ctl(epollFd,
        EPOLL_CTL_ADD,
        eventFd,
        &event);

    printf("Sending event...\n");

    uint64_t value = 5;

    write(eventFd,
        &value,
        sizeof(value));

    int count = epoll_wait(epollFd,
        events,
        MAX_EVENTS,
        -1);

    if (count > 0)
    {
        uint64_t received;

        read(eventFd,
            &received,
            sizeof(received));

        printf("eventfd value = %lu\n",
            received);
    }

    close(eventFd);
    close(epollFd);

    return EXIT_SUCCESS;
}



