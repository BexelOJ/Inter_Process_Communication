#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/ipc_fifo_client"

int main(void)
{
    int fd;
    char buffer[100];

    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        perror("mkfifo");
    }

    printf("FIFO server started.\n");
    printf("Waiting for client...\n");

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

        printf("Server received: %s", buffer);
    }

    close(fd);

    unlink(FIFO_PATH);

    return 0;
}


/*
//---------------------------------------------------
FIFO server corresponding to the client.

//---------------------------------------------------
*/


