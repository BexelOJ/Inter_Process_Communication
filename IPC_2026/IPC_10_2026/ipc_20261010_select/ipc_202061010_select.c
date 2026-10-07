#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void)
{
    int serverFd;
    int clientFd;

    struct sockaddr_in serverAddr;

    fd_set masterSet;
    fd_set readSet;

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

    FD_ZERO(&masterSet);
    FD_SET(serverFd, &masterSet);

    int maxFd = serverFd;

    printf("select() server listening on %d\n", PORT);

    while (1)
    {
        readSet = masterSet;

        int activity = select(maxFd + 1,
            &readSet,
            NULL,
            NULL,
            NULL);

        if (activity < 0)
        {
            perror("select");
            break;
        }

        for (int fd = 0; fd <= maxFd; fd++)
        {
            if (!FD_ISSET(fd, &readSet))
                continue;

            if (fd == serverFd)
            {
                clientFd = accept(serverFd, NULL, NULL);

                if (clientFd < 0)
                {
                    perror("accept");
                    continue;
                }

                FD_SET(clientFd, &masterSet);

                if (clientFd > maxFd)
                    maxFd = clientFd;

                printf("New client: fd=%d\n", clientFd);
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
                    printf("Client disconnected: fd=%d\n", fd);

                    close(fd);
                    FD_CLR(fd, &masterSet);
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



