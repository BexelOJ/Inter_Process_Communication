#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_PATH "/tmp/ipc_fifo_client"

int main(void)
{
    int fd;
    char message[100];

    printf("Enter message: ");

    if (fgets(message, sizeof(message), stdin) == NULL)
    {
        return 1;
    }

    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        perror("mkfifo");
    }

    fd = open(FIFO_PATH, O_WRONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    write(fd, message, strlen(message));

    printf("Client sent: %s", message);

    close(fd);

    return 0;
}


/*
//---------------------------------------------------
Simple FIFO client.


Architecture:

Client
   |
   | write()
   v
FIFO
   |
   v
Server

//---------------------------------------------------
*/


