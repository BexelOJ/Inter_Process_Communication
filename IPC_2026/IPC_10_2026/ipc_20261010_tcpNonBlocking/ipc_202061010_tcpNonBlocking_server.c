#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void)
{
    int serverFd;
    int clientFd;

    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    int flags = fcntl(serverFd, F_GETFL, 0);

    fcntl(serverFd, F_SETFL, flags | O_NONBLOCK);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverFd,
        (struct sockaddr*)&serverAddr,
        sizeof(serverAddr));

    listen(serverFd, 5);

    printf("Non-blocking server started.\n");

    while (1)
    {
        clientFd = accept(serverFd, NULL, NULL);

        if (clientFd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                printf("No client currently available.\n");
                usleep(500000);
                continue;
            }

            perror("accept");
            break;
        }

        printf("Client connected.\n");

        int clientFlags = fcntl(clientFd, F_GETFL, 0);

        fcntl(clientFd,
            F_SETFL,
            clientFlags | O_NONBLOCK);

        while (1)
        {
            memset(buffer, 0, sizeof(buffer));

            int bytes = recv(clientFd,
                buffer,
                sizeof(buffer) - 1,
                0);

            if (bytes > 0)
            {
                printf("Client: %s", buffer);

                send(clientFd, buffer, bytes, 0);
            }
            else if (bytes == 0)
            {
                printf("Client disconnected.\n");
                break;
            }
            else
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    usleep(100000);
                    continue;
                }

                perror("recv");
                break;
            }
        }

        close(clientFd);
    }

    close(serverFd);

    return EXIT_SUCCESS;
}



