#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/ipc_fifo_blocking"

int main(void)
{
    int fd;

    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        perror("mkfifo");
    }

    printf("Opening FIFO for reading...\n");
    printf("Program will block until a writer opens the FIFO.\n");

    fd = open(FIFO_PATH, O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    printf("Writer connected.\n");

    close(fd);

    unlink(FIFO_PATH);

    return 0;
}


/*
//---------------------------------------------------

Demonstrates FIFO blocking behavior

//---------------------------------------------------
*/


