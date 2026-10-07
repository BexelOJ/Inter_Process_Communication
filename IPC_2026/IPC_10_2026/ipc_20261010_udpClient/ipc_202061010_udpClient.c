#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void)
{
    int sockFd;

    struct sockaddr_in serverAddr;

    socklen_t serverLen = sizeof(serverAddr);

    char buffer[BUFFER_SIZE];

    sockFd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockFd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    inet_pton(AF_INET,
        "127.0.0.1",
        &serverAddr.sin_addr);

    while (1)
    {
        printf("Enter message: ");

        if (!fgets(buffer,
            sizeof(buffer),
            stdin))
            break;

        sendto(sockFd,
            buffer,
            strlen(buffer),
            0,
            (struct sockaddr*)&serverAddr,
            serverLen);

        memset(buffer, 0, sizeof(buffer));

        int bytes = recvfrom(sockFd,
            buffer,
            sizeof(buffer) - 1,
            0,
            NULL,
            NULL);

        if (bytes < 0)
        {
            perror("recvfrom");
            break;
        }

        printf("Server: %s", buffer);
    }

    close(sockFd);

    return EXIT_SUCCESS;
}



