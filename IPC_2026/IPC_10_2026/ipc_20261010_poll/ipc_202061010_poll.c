#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main(void)
{
    int serverFd;
    int clientFd;

    struct sockaddr_in serverAddr;

    struct pollfd fds[MAX_CLIENTS + 1];

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

    bind(serverFd,
        (struct sockaddr*)&serverAddr,
        sizeof(serverAddr));

    listen(serverFd, 10);

    for (int i = 0; i <= MAX_CLIENTS; i++)
        fds[i].fd = -1;

    fds[0].fd = serverFd;
    fds[0].events = POLLIN;

    printf("poll() server listening on %d\n", PORT);

    while (1)
    {
        int activity = poll(fds,
            MAX_CLIENTS + 1,
            -1);

        if (activity < 0)
        {
            perror("poll");
            break;
        }

        if (fds[0].revents & POLLIN)
        {
            clientFd = accept(serverFd, NULL, NULL);

            if (clientFd < 0)
            {
                perror("accept");
                continue;
            }

            printf("New client fd=%d\n", clientFd);

            int added = 0;

            for (int i = 1; i <= MAX_CLIENTS; i++)
            {
                if (fds[i].fd == -1)
                {
                    fds[i].fd = clientFd;
                    fds[i].events = POLLIN;
                    added = 1;
                    break;
                }
            }

            if (!added)
            {
                printf("Maximum clients reached.\n");
                close(clientFd);
            }
        }

        for (int i = 1; i <= MAX_CLIENTS; i++)
        {
            if (fds[i].fd == -1)
                continue;

            if (fds[i].revents & POLLIN)
            {
                int fd = fds[i].fd;

                memset(buffer, 0, sizeof(buffer));

                int bytes = recv(fd,
                    buffer,
                    sizeof(buffer) - 1,
                    0);

                if (bytes <= 0)
                {
                    printf("Client disconnected fd=%d\n", fd);

                    close(fd);
                    fds[i].fd = -1;
                }
                else
                {
                    printf("fd=%d: %s", fd, buffer);

                    send(fd, buffer, bytes, 0);
                }
            }
        }
    }

    close(serverFd);

    return EXIT_SUCCESS;
}



