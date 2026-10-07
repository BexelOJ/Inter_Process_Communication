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
    struct sockaddr_in clientAddr;

    socklen_t clientLen = sizeof(clientAddr);

    char buffer[BUFFER_SIZE];

    sockFd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockFd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(sockFd,
        (struct sockaddr*)&serverAddr,
        sizeof(serverAddr)) < 0)
    {
        perror("bind");
        close(sockFd);
        return EXIT_FAILURE;
    }

    printf("UDP server listening on %d\n", PORT);

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytes = recvfrom(sockFd,
            buffer,
            sizeof(buffer) - 1,
            0,
            (struct sockaddr*)&clientAddr,
            &clientLen);

        if (bytes < 0)
        {
            perror("recvfrom");
            break;
        }

        printf("Client: %s", buffer);

        sendto(sockFd,
            buffer,
            bytes,
            0,
            (struct sockaddr*)&clientAddr,
            clientLen);
    }

    close(sockFd);

    return EXIT_SUCCESS;
}



