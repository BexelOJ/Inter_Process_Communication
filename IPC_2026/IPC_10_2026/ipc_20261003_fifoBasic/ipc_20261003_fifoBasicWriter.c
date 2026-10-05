#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_NAME "/tmp/ipc_fifo_basic"

int main(void)
{
    int fd;
    const char *message = "Hello from FIFO writer";

    // Create FIFO
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

    write(fd, message, strlen(message) + 1);

    close(fd);

    printf("Writer finished.\n");

    return EXIT_SUCCESS;
}



