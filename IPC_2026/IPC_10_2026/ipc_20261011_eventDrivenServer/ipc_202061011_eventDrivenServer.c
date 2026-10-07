#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 64
#define BUFFER_SIZE 1024

enum EventType
{
    EVENT_SERVER,
    EVENT_CLIENT
};

struct Connection
{
    int fd;
    enum EventType type;
};

int main(void)
{
    int serverFd;
    int epollFd;

    struct sockaddr_in serverAddr;

    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverFd,
        (struct sockaddr*)&serverAddr,
        sizeof(serverAddr));

    listen(serverFd, 10);

    epollFd = epoll_create1(0);

    if (epollFd < 0)
    {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    struct Connection serverConnection;

    serverConnection.fd = serverFd;
    serverConnection.type = EVENT_SERVER;

    struct epoll_event event;

    event.events = EPOLLIN;
    event.data.ptr = &serverConnection;

    epoll_ctl(epollFd,
        EPOLL_CTL_ADD,
        serverFd,
        &event);

    printf("Event-driven server started.\n");

    while (1)
    {
        struct epoll_event events[MAX_EVENTS];

        int count = epoll_wait(epollFd,
            events,
            MAX_EVENTS,
            -1);

        for (int i = 0; i < count; i++)
        {
            struct Connection* connection =
                events[i].data.ptr;

            if (connection->type == EVENT_SERVER)
            {
                int clientFd =
                    accept(serverFd,
                        NULL,
                        NULL);

                if (clientFd < 0)
                {
                    perror("accept");
                    continue;
                }

                struct Connection* client =
                    malloc(sizeof(struct Connection));

                client->fd = clientFd;
                client->type = EVENT_CLIENT;

                event.events = EPOLLIN;
                event.data.ptr = client;

                epoll_ctl(epollFd,
                    EPOLL_CTL_ADD,
                    clientFd,
                    &event);

                printf("EVENT: CLIENT_CONNECTED fd=%d\n",
                    clientFd);
            }
            else if (connection->type == EVENT_CLIENT)
            {
                char buffer[BUFFER_SIZE];

                int bytes = recv(connection->fd,
                    buffer,
                    sizeof(buffer) - 1,
                    0);

                if (bytes <= 0)
                {
                    printf("EVENT: CLIENT_DISCONNECTED fd=%d\n",
                        connection->fd);

                    epoll_ctl(epollFd,
                        EPOLL_CTL_DEL,
                        connection->fd,
                        NULL);

                    close(connection->fd);

                    free(connection);
                }
                else
                {
                    buffer[bytes] = '\0';

                    printf("EVENT: DATA fd=%d: %s",
                        connection->fd,
                        buffer);

                    send(connection->fd,
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



