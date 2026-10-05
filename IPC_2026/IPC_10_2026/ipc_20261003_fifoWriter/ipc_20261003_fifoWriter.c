#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_NAME "/tmp/ipc_fifo_writer"

int main(void)
{
    int fd;
    const char *message = "Hello from FIFO Writer";

    /* Create FIFO */
    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        perror("mkfifo");
    }

    printf("Opening FIFO for writing...\n");

    fd = open(FIFO_NAME, O_WRONLY);

    if (fd == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    printf("Writing: %s\n", message);

    if (write(fd, message, strlen(message) + 1) == -1)
    {
        perror("write");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Data written successfully.\n");

    close(fd);

    return EXIT_SUCCESS;
}



