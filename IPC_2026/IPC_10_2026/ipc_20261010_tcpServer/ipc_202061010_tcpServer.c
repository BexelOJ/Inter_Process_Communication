#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

    listen(serverFd, 5);

    printf("Server listening on port %d\n", PORT);

    clientFd = accept(serverFd, NULL, NULL);

    if (clientFd < 0)
    {
        perror("accept");
        close(serverFd);
        return EXIT_FAILURE;
    }

    printf("Client connected.\n");

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

        if (bytes <= 0)
            break;

        printf("Received: %s", buffer);

        send(clientFd, buffer, bytes, 0);
    }

    close(clientFd);
    close(serverFd);

    return EXIT_SUCCESS;
}



