#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define FIFO_NAME "/tmp/ipc_fifo_nonblocking"

int main(void)
{
    int fifoId;
    char buffer[256];

    /* Create FIFO */
    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");
            return EXIT_FAILURE;
        }
    }

    printf("Opening FIFO in non-blocking mode...\n");

    /*
     * O_NONBLOCK:
     * open() will not wait for a writer.
     */
    fifoId = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);

    if (fifoId == -1)
    {
        perror("open");
        unlink(FIFO_NAME);
        return EXIT_FAILURE;
    }

    printf("FIFO opened successfully.\n");

    /*
     * Try to read without blocking.
     */
    ssize_t bytesRead = read(
        fifoId,
        buffer,
        sizeof(buffer) - 1
    );

    if (bytesRead == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            printf("No data available right now.\n");
        }
        else
        {
            perror("read");
        }
    }
    else if (bytesRead == 0)
    {
        printf("No writer is connected.\n");
    }
    else
    {
        buffer[bytesRead] = '\0';

        printf("Received: %s\n", buffer);
    }

    close(fifoId);

    unlink(FIFO_NAME);

    return EXIT_SUCCESS;
}



