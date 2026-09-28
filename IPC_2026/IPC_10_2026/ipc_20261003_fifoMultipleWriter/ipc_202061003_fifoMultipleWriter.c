#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define FIFO_PATH "/tmp/ipc_fifo_multiple_writer"
#define WRITER_COUNT 3

int main(void)
{
    int fd;

    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        perror("mkfifo");
    }

    /*
     * Open read/write so that the parent itself
     * keeps both ends alive.
     */
    fd = open(FIFO_PATH, O_RDWR);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    for (int i = 0; i < WRITER_COUNT; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            return 1;
        }

        if (pid == 0)
        {
            char message[100];

            snprintf(message,
                sizeof(message),
                "Message from writer %d, PID=%d\n",
                i + 1,
                getpid());

            write(fd, message, strlen(message));

            close(fd);

            exit(0);
        }
    }

    /*
     * Parent reads messages from all writers.
     */
    char buffer[100];

    for (int i = 0; i < WRITER_COUNT; i++)
    {
        ssize_t bytesRead;

        bytesRead = read(fd, buffer, sizeof(buffer) - 1);

        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';

            printf("Reader received: %s", buffer);
        }
    }

    for (int i = 0; i < WRITER_COUNT; i++)
    {
        wait(NULL);
    }

    close(fd);

    unlink(FIFO_PATH);

    return 0;
}


/*
//---------------------------------------------------
Multiple writer processes write to one FIFO.


Architecture:

Writer 1 ──┐
            │
Writer 2 ──┼──> FIFO ──> Reader
            │
Writer 3 ──┘

//---------------------------------------------------
*/


