#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_NAME "/tmp/ipc_fifo_server"

int main(void)
{
    int fd;
    char buffer[256];

    /* Create FIFO */
    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        perror("mkfifo");
    }

    printf("FIFO Server started.\n");
    printf("Waiting for client messages...\n");

    while (1)
    {
        /*
         * Open FIFO for reading.
         *
         * This blocks until a writer opens the FIFO.
         */
        fd = open(FIFO_NAME, O_RDONLY);

        if (fd == -1)
        {
            perror("open");
            unlink(FIFO_NAME);
            return EXIT_FAILURE;
        }

        ssize_t bytesRead = read(
            fd,
            buffer,
            sizeof(buffer) - 1
        );

        close(fd);

        if (bytesRead == -1)
        {
            perror("read");
            continue;
        }

        if (bytesRead == 0)
        {
            continue;
        }

        buffer[bytesRead] = '\0';

        printf("Server received: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0)
        {
            printf("Server shutting down.\n");
            break;
        }
    }

    unlink(FIFO_NAME);

    return EXIT_SUCCESS;
}


// FROM ANOTHER TERMINAL:

// echo "Hello Server" > /tmp/ipc_fifo_server
// echo "This is message 2" > /tmp/ipc_fifo_server
// echo "exit" > /tmp/ipc_fifo_server



