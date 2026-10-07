#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

int main(void)
{
    int timerFd;
    int epollFd;

    struct itimerspec timerSpec;

    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];

    timerFd = timerfd_create(CLOCK_MONOTONIC, 0);

    if (timerFd < 0)
    {
        perror("timerfd_create");
        return EXIT_FAILURE;
    }

    /*
     * First expiration:
     * 2 seconds
     */
    timerSpec.it_value.tv_sec = 2;
    timerSpec.it_value.tv_nsec = 0;

    /*
     * Repeat every 1 second.
     */
    timerSpec.it_interval.tv_sec = 1;
    timerSpec.it_interval.tv_nsec = 0;

    if (timerfd_settime(timerFd,
        0,
        &timerSpec,
        NULL) < 0)
    {
        perror("timerfd_settime");
        return EXIT_FAILURE;
    }

    epollFd = epoll_create1(0);

    if (epollFd < 0)
    {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    event.events = EPOLLIN;
    event.data.fd = timerFd;

    epoll_ctl(epollFd,
        EPOLL_CTL_ADD,
        timerFd,
        &event);

    printf("Timer started...\n");

    while (1)
    {
        int count = epoll_wait(epollFd,
            events,
            MAX_EVENTS,
            -1);

        if (count < 0)
        {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < count; i++)
        {
            if (events[i].data.fd == timerFd)
            {
                uint64_t expirations;

                read(timerFd,
                    &expirations,
                    sizeof(expirations));

                printf("Timer expired: %lu time(s)\n",
                    expirations);
            }
        }
    }

    close(timerFd);
    close(epollFd);

    return EXIT_SUCCESS;
}



