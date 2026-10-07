#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 64
#define BUFFER_SIZE 1024

int createServer(void)
{
    int serverFd;

    struct sockaddr_in serverAddr;

    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }

    if (listen(serverFd, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return serverFd;
}

void handleClient(int epollFd, int clientFd)
{
    char buffer[BUFFER_SIZE];

    memset(buffer, 0, sizeof(buffer));

    int bytes = recv(clientFd,
        buffer,
        sizeof(buffer) - 1,
        0);

    if (bytes <= 0)
    {
        printf("Client disconnected: %d\n",
            clientFd);

        epoll_ctl(epollFd,
            EPOLL_CTL_DEL,
            clientFd,
            NULL);

        close(clientFd);

        return;
    }

    printf("Client %d: %s",
        clientFd,
        buffer);

    send(clientFd,
        buffer,
        bytes,
        0);
}

int main(void)
{
    int serverFd = createServer();

    int epollFd = epoll_create1(0);

    if (epollFd < 0)
    {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];

    event.events = EPOLLIN;
    event.data.fd = serverFd;

    epoll_ctl(epollFd,
        EPOLL_CTL_ADD,
        serverFd,
        &event);

    printf("epoll server listening on %d\n",
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

                printf("New client: %d\n",
                    clientFd);
            }
            else
            {
                handleClient(epollFd, fd);
            }
        }
    }

    close(epollFd);
    close(serverFd);

    return EXIT_SUCCESS;
}



