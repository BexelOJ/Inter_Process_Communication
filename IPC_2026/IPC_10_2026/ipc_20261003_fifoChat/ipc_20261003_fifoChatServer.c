#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_CLIENT "/tmp/fifo_client"
#define FIFO_SERVER "/tmp/fifo_server"

int main(void)
{
    int readFd;
    int writeFd;

    char buffer[256];

    mkfifo(FIFO_CLIENT, 0666);
    mkfifo(FIFO_SERVER, 0666);

    printf("FIFO chat server started.\n");

    // Open client -> server
    readFd = open(FIFO_CLIENT, O_RDONLY);

    if (readFd == -1)
    {
        perror("open client FIFO");
        return EXIT_FAILURE;
    }

    // Open server -> client
    writeFd = open(FIFO_SERVER, O_WRONLY);

    if (writeFd == -1)
    {
        perror("open server FIFO");
        return EXIT_FAILURE;
    }

    while (1)
    {
        printf("Client: ");

        ssize_t bytesRead = read(readFd, buffer, sizeof(buffer) - 1);

        if (bytesRead <= 0)
            break;

        buffer[bytesRead] = '\0';

        printf("%s\n", buffer);

        if (strcmp(buffer, "exit") == 0)
            break;

        printf("Server: ");

        fgets(buffer, sizeof(buffer), stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        write(writeFd, buffer, strlen(buffer) + 1);

        if (strcmp(buffer, "exit") == 0)
            break;
    }

    close(readFd);
    close(writeFd);

    unlink(FIFO_CLIENT);
    unlink(FIFO_SERVER);

    return EXIT_SUCCESS;
}



