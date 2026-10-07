#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

int main(void)
{
    int serverFd;
    int clientFd;
    int epollFd;

    struct sockaddr_in serverAddr;

    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];

    char buffer[BUFFER_SIZE];

    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverFd,
        (struct sockaddr*)&serverAddr,
        sizeof(serverAddr)) < 0)
    {
        perror("bind");
        close(serverFd);
        return EXIT_FAILURE;
    }

    listen(serverFd, 10);

    epollFd = epoll_create1(0);

    if (epollFd < 0)
    {
        perror("epoll_create1");
        close(serverFd);
        return EXIT_FAILURE;
    }

    event.events = EPOLLIN;
    event.data.fd = serverFd;

    epoll_ctl(epollFd,
        EPOLL_CTL_ADD,
        serverFd,
        &event);

    printf("epoll server listening on %d\n", PORT);

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
            int fd = events[i].data.fd;

            if (fd == serverFd)
            {
                clientFd = accept(serverFd,
                    NULL,
                    NULL);

                if (clientFd < 0)
                {
                    perror("accept");
                    continue;
                }

                event.events = EPOLLIN;
                event.data.fd = clientFd;

                epoll_ctl(epollFd,
                    EPOLL_CTL_ADD,
                    clientFd,
                    &event);

                printf("New client fd=%d\n", clientFd);
            }
            else
            {
                memset(buffer, 0, sizeof(buffer));

                int bytes = recv(fd,
                    buffer,
                    sizeof(buffer) - 1,
                    0);

                if (bytes <= 0)
                {
                    printf("Client disconnected fd=%d\n", fd);

                    epoll_ctl(epollFd,
                        EPOLL_CTL_DEL,
                        fd,
                        NULL);

                    close(fd);
                }
                else
                {
                    printf("fd=%d: %s", fd, buffer);

                    send(fd, buffer, bytes, 0);
                }
            }
        }
    }

    close(epollFd);
    close(serverFd);

    return EXIT_SUCCESS;
}


