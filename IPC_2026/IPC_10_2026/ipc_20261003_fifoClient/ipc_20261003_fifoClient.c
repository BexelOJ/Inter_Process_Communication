#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "/tmp/ipc_fifo_server"

int main(void)
{
    int fifoId;
    char message[256];

    printf("FIFO Client started.\n");

    /*
     * Open FIFO for writing.
     *
     * This will block until the server
     * opens the FIFO for reading.
     */
    printf("Opening FIFO...\n");

    fifoId = open(FIFO_NAME, O_WRONLY);

    if (fifoId == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    printf("Connected to FIFO server.\n");

    while (1)
    {
        printf("Client: ");

        if (fgets(message, sizeof(message), stdin) == NULL)
        {
            break;
        }

        /* Remove newline */
        message[strcspn(message, "\n")] = '\0';

        /*
         * Send message to server.
         */
        if (write(fifoId, message, strlen(message) + 1) == -1)
        {
            perror("write");
            break;
        }

        /*
         * Exit condition.
         */
        if (strcmp(message, "exit") == 0)
        {
            break;
        }
    }

    close(fifoId);

    printf("FIFO Client terminated.\n");

    return EXIT_SUCCESS;
}



