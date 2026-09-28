#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_PATH "/tmp/ipc_fifo_basic"

int main(void)
{
    int fd;
    char buffer[100];

    /* Create FIFO */
    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        perror("mkfifo");
    }

    /*
     * Open FIFO for writing.
     *
     * Since there is no reader yet, this may block.
     */
    fd = open(FIFO_PATH, O_WRONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    const char* message = "Hello from FIFO";

    write(fd, message, strlen(message) + 1);

    close(fd);

    /*
     * This example demonstrates creation and writing.
     * A separate reader is required to consume the data.
     */

    printf("Message written to FIFO\n");

    return 0;
}


/*
//---------------------------------------------------
Basic FIFO creation and communication 
within a single process.

Because 
open(..., O_WRONLY) waits for a reader, 
this program is normally used together 
with a FIFO reader.

//---------------------------------------------------
*/


