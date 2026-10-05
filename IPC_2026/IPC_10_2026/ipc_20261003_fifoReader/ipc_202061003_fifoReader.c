#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/ipc_fifo_reader"

int main(void)
{
    int fd;
    char buffer[256];

    /* Create FIFO */
    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        perror("mkfifo");
    }

    printf("Opening FIFO for reading...\n");

    fd = open(FIFO_NAME, O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    printf("Waiting for data...\n");

    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);

    if (bytesRead == -1)
    {
        perror("read");
        close(fd);
        return EXIT_FAILURE;
    }

    buffer[bytesRead] = '\0';

    printf("Received: %s\n", buffer);

    close(fd);

    /* Remove FIFO */
    unlink(FIFO_NAME);

    return EXIT_SUCCESS;
}



