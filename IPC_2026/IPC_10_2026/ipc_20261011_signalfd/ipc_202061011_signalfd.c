#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

int main(void)
{
    int signalFd;
    int epollFd;

    sigset_t mask;

    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];

    sigemptyset(&mask);

    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);

    /*
     * Block these signals.
     *
     * They will instead be received
     * through signalfd.
     */
    if (sigprocmask(SIG_BLOCK,
        &mask,
        NULL) < 0)
    {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    signalFd = signalfd(-1,
        &mask,
        0);

    if (signalFd < 0)
    {
        perror("signalfd");
        return EXIT_FAILURE;
    }

    epollFd = epoll_create1(0);

    if (epollFd < 0)
    {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    event.events = EPOLLIN;
    event.data.fd = signalFd;

    epoll_ctl(epollFd,
        EPOLL_CTL_ADD,
        signalFd,
        &event);

    printf("Waiting for SIGINT or SIGTERM...\n");
    printf("Press Ctrl+C\n");

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
            if (events[i].data.fd == signalFd)
            {
                struct signalfd_siginfo signalInfo;

                ssize_t bytes =
                    read(signalFd,
                        &signalInfo,
                        sizeof(signalInfo));

                if (bytes != sizeof(signalInfo))
                    continue;

                printf("Signal received: %d\n",
                    signalInfo.ssi_signo);

                if (signalInfo.ssi_signo == SIGINT ||
                    signalInfo.ssi_signo == SIGTERM)
                {
                    printf("Exiting...\n");
                    goto cleanup;
                }
            }
        }
    }

cleanup:

    close(signalFd);
    close(epollFd);

    return EXIT_SUCCESS;
}



