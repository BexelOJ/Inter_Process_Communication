#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_A "/tmp/ipc_fifo_chat_a"
#define FIFO_B "/tmp/ipc_fifo_chat_b"

int main(void)
{
    int writeFd;
    int readFd;

    char message[100];
    char reply[100];

    /*
     * Create two FIFOs.
     *
     * FIFO_A:
     *     Process A -> Process B
     *
     * FIFO_B:
     *     Process B -> Process A
     */

    if (mkfifo(FIFO_A, 0666) == -1)
    {
        perror("mkfifo A");
    }

    if (mkfifo(FIFO_B, 0666) == -1)
    {
        perror("mkfifo B");
    }

    printf("FIFO chat started.\n");

    /*
     * Open both FIFOs.
     *
     * O_RDWR avoids blocking during startup.
     */
    writeFd = open(FIFO_A, O_RDWR);

    if (writeFd == -1)
    {
        perror("open FIFO_A");
        return 1;
    }

    readFd = open(FIFO_B, O_RDWR);

    if (readFd == -1)
    {
        perror("open FIFO_B");
        return 1;
    }

    while (1)
    {
        printf("\nEnter message: ");

        if (fgets(message, sizeof(message), stdin) == NULL)
        {
            break;
        }

        if (strncmp(message, "exit", 4) == 0)
        {
            break;
        }

        write(writeFd, message, strlen(message));

        ssize_t bytesRead;

        bytesRead = read(readFd,
            reply,
            sizeof(reply) - 1);

        if (bytesRead > 0)
        {
            reply[bytesRead] = '\0';

            printf("Received: %s", reply);
        }
    }

    close(writeFd);
    close(readFd);

    unlink(FIFO_A);
    unlink(FIFO_B);

    return 0;
}


/*
//---------------------------------------------------
A simple two-way chat using two FIFOs.

For a real two-process chat, 
both processes need complementary FIFO directions:

Process A                         Process B

FIFO_A
write ──────────────────────────> read


FIFO_B
read  <────────────────────────── write

The above program demonstrates the underlying mechanism; 
for a fully interactive chat, 
I'd make separate fifoChatServer.c 
and fifoChatClient.c programs

//---------------------------------------------------
*/


