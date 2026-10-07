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
    char buffer[BUFFER_SIZE];

    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sockFd,
                (struct sockaddr *)&serverAddr,
                sizeof(serverAddr)) < 0)
    {
        perror("connect");
        close(sockFd);
        return EXIT_FAILURE;
    }

    printf("Connected to server.\n");

    while (1)
    {
        printf("Enter message: ");

        if (!fgets(buffer, sizeof(buffer), stdin))
            break;

        write(sockFd, buffer, strlen(buffer));

        memset(buffer, 0, sizeof(buffer));

        int bytes = read(sockFd, buffer, sizeof(buffer) - 1);

        if (bytes <= 0)
            break;

        printf("Server: %s", buffer);
    }

    close(sockFd);

    return EXIT_SUCCESS;
}
