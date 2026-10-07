#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 64
#define BUFFER_SIZE 1024

int main(void)
{
    int serverFd;
    int epollFd;

    struct sockaddr_in serverAddr;

    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];

    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    int opt = 1;

    setsockopt(serverFd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverFd,
        (struct sockaddr*)&serverAddr,
        sizeof(serverAddr)) < 0)
    {
        perror("bind");
        return EXIT_FAILURE;
    }

    if (listen(serverFd, 10) < 0)
    {
        perror("listen");
        return EXIT_FAILURE;
    }

    epollFd = epoll_create1(0);

    if (epollFd < 0)
    {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    event.events = EPOLLIN;
    event.data.fd = serverFd;

    epoll_ctl(epollFd,
        EPOLL_CTL_ADD,
        serverFd,
        &event);

    printf("epoll multi-client server on port %d\n",
        PORT);

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
                int clientFd = accept(serverFd,
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

                printf("Client connected fd=%d\n",
                    clientFd);
            }
            else
            {
                char buffer[BUFFER_SIZE];

                memset(buffer, 0, sizeof(buffer));

                int bytes = recv(fd,
                    buffer,
                    sizeof(buffer) - 1,
                    0);

                if (bytes <= 0)
                {
                    printf("Client disconnected fd=%d\n",
                        fd);

                    epoll_ctl(epollFd,
                        EPOLL_CTL_DEL,
                        fd,
                        NULL);

                    close(fd);
                }
                else
                {
                    printf("Client %d: %s",
                        fd,
                        buffer);

                    send(fd,
                        buffer,
                        bytes,
                        0);
                }
            }
        }
    }

    close(epollFd);
    close(serverFd);

    return EXIT_SUCCESS;
}



