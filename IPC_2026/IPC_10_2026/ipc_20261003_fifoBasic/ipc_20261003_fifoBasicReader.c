#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/ipc_fifo_basic"

int main(void)
{
    int fd;
    char buffer[256];

    printf("Opening FIFO for reading...\n");

    fd = open(FIFO_NAME, O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    read(fd, buffer, sizeof(buffer));

    printf("Received: %s\n", buffer);

    close(fd);

    // Remove FIFO
    unlink(FIFO_NAME);

    return EXIT_SUCCESS;
}



