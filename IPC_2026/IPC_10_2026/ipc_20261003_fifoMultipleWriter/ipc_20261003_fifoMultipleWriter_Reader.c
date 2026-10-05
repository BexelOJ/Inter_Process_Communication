#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/ipc_fifo_multi"

int main(void)
{
    int fd;
    char buffer[256];

    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        perror("mkfifo");
    }

    printf("Reader waiting...\n");

    fd = open(FIFO_NAME, O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    while (1)
    {
        ssize_t bytesRead;

        bytesRead = read(fd, buffer, sizeof(buffer) - 1);

        if (bytesRead <= 0)
            break;

        buffer[bytesRead] = '\0';

        printf("Reader received: %s\n", buffer);
    }

    close(fd);

    unlink(FIFO_NAME);

    return EXIT_SUCCESS;
}



