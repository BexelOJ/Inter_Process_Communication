#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_PATH "/tmp/ipc_fifo_writer"

int main(void)
{
    int fd;

    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        perror("mkfifo");
    }

    printf("Opening FIFO for writing...\n");

    fd = open(FIFO_PATH, O_WRONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    const char* message = "Hello from FIFO writer";

    write(fd, message, strlen(message) + 1);

    printf("Message sent\n");

    close(fd);

    return 0;
}


/*
//---------------------------------------------------
Dedicated FIFO writer.

//---------------------------------------------------
*/


