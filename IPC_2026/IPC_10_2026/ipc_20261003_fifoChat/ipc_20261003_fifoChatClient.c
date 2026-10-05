#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_CLIENT "/tmp/fifo_client"
#define FIFO_SERVER "/tmp/fifo_server"

int main(void)
{
    int writeFd;
    int readFd;

    char buffer[256];

    printf("FIFO chat client started.\n");

    // Client -> server
    writeFd = open(FIFO_CLIENT, O_WRONLY);

    if (writeFd == -1)
    {
        perror("open client FIFO");
        return EXIT_FAILURE;
    }

    // Server -> client
    readFd = open(FIFO_SERVER, O_RDONLY);

    if (readFd == -1)
    {
        perror("open server FIFO");
        return EXIT_FAILURE;
    }

    while (1)
    {
        printf("Client: ");

        fgets(buffer, sizeof(buffer), stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        write(writeFd, buffer, strlen(buffer) + 1);

        if (strcmp(buffer, "exit") == 0)
            break;

        ssize_t bytesRead = read(readFd, buffer, sizeof(buffer) - 1);

        if (bytesRead <= 0)
            break;

        buffer[bytesRead] = '\0';

        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0)
            break;
    }

    close(writeFd);
    close(readFd);

    return EXIT_SUCCESS;
}



