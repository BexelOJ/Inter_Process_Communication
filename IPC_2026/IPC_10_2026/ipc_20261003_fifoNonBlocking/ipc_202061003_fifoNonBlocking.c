#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FIFO_PATH "/tmp/ipc_fifo_nonblocking"

int main(void)
{
    int fd;

    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        perror("mkfifo");
    }

    printf("Opening FIFO in non-blocking mode...\n");

    fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);

    if (fd == -1)
    {
        perror("open");
        unlink(FIFO_PATH);
        return 1;
    }

    printf("FIFO opened without waiting for writer.\n");

    char buffer[100];

    ssize_t bytesRead;

    bytesRead = read(fd, buffer, sizeof(buffer) - 1);

    if (bytesRead == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            printf("No data available.\n");
            printf("read() did not block.\n");
        }
        else
        {
            perror("read");
        }
    }
    else if (bytesRead == 0)
    {
        printf("EOF received.\n");
    }
    else
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
Uses O_NONBLOCK


Important difference:

Blocking:

open/read
   ↓
wait


Non-blocking:

open/read
   ↓
return immediately
   ↓
EAGAIN

//---------------------------------------------------
*/


