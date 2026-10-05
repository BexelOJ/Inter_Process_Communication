#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/ipc_fifo_blocking"

int main(void)
{
    int fd;
    char buffer[100];

    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        perror("mkfifo");
    }

    printf("Opening FIFO...\n");
    printf("Program will block here until another process opens FIFO.\n");

    fd = open(FIFO_NAME, O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    printf("FIFO opened successfully.\n");

    printf("Waiting for data...\n");

    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);

    if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';

        printf("Received: %s\n", buffer);
    }

    close(fd);

    unlink(FIFO_NAME);

    return EXIT_SUCCESS;

}


// echo "Hello FIFO" > /tmp/ipc_fifo_blocking
// in another terminal

