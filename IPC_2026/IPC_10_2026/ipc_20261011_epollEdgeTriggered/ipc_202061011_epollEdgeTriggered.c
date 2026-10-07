#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
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

    int flags = fcntl(pipeFd[0], F_GETFL, 0);

    fcntl(pipeFd[0],
        F_SETFL,
        flags | O_NONBLOCK);

    epollFd = epoll_create1(0);

    if (epollFd < 0)
    {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    event.events = EPOLLIN | EPOLLET;
    event.data.fd = pipeFd[0];

    epoll_ctl(epollFd,
        EPOLL_CTL_ADD,
        pipeFd[0],
        &event);

    write(pipeFd[1],
        "ABCDEFGHIJ",
        10);

    int count = epoll_wait(epollFd,
        events,
        MAX_EVENTS,
        1000);

    printf("epoll_wait() returned %d\n", count);

    if (count > 0)
    {
        while (1)
        {
            int bytes = read(pipeFd[0],
                buffer,
                sizeof(buffer));

            if (bytes > 0)
            {
                printf("Read %d bytes: ", bytes);

                write(STDOUT_FILENO,
                    buffer,
                    bytes);

                printf("\n");
            }
            else if (bytes == -1 &&
                (errno == EAGAIN ||
                    errno == EWOULDBLOCK))
            {
                printf("Reached EAGAIN.\n");
                break;
            }
            else if (bytes == 0)
            {
                printf("Pipe closed.\n");
                break;
            }
            else
            {
                perror("read");
                break;
            }
        }
    }

    close(pipeFd[0]);
    close(pipeFd[1]);
    close(epollFd);

    return EXIT_SUCCESS;
}



