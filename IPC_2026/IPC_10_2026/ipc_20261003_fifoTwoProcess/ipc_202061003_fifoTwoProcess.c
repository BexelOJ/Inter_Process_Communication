#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_PATH "/tmp/ipc_fifo_two_process"

int main(void)
{
    int fd;
    char buffer[100];

    /*
     * Both processes use the same FIFO path.
     */

    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        perror("mkfifo");
    }

    printf("Opening FIFO for reading...\n");

    fd = open(FIFO_PATH, O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    ssize_t bytesRead;

    bytesRead = read(fd, buffer, sizeof(buffer) - 1);

    if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';

        printf("Received: %s\n", buffer);
    }

    close(fd);

    unlink(FIFO_PATH);

    return 0;
}


/*
//---------------------------------------------------
Two unrelated processes communicating through a FIFO.

This is the important distinction 
from your unnamed-pipe examples.

//---------------------------------------------------
*/


