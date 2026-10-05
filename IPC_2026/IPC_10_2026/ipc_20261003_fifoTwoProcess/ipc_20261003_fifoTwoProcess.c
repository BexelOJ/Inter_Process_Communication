#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

#define FIFO_NAME "/tmp/ipc_fifo_two_process"

int main(void)
{
    int fd;
    pid_t pid;

    char buffer[256];

    /* Create FIFO */
    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        perror("mkfifo");
    }

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        unlink(FIFO_NAME);
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        /* Child process - Reader */

        printf("Child: opening FIFO for reading...\n");

        fd = open(FIFO_NAME, O_RDONLY);

        if (fd == -1)
        {
            perror("child open");
            exit(EXIT_FAILURE);
        }

        printf("Child: waiting for data...\n");

        ssize_t bytesRead = read(
            fd,
            buffer,
            sizeof(buffer) - 1
        );

        if (bytesRead == -1)
        {
            perror("child read");
            close(fd);
            exit(EXIT_FAILURE);
        }

        buffer[bytesRead] = '\0';

        printf("Child received: %s\n", buffer);

        close(fd);

        exit(EXIT_SUCCESS);
    }
    else
    {
        /* Parent process - Writer */

        sleep(1);

        printf("Parent: opening FIFO for writing...\n");

        fd = open(FIFO_NAME, O_WRONLY);

        if (fd == -1)
        {
            perror("parent open");
            unlink(FIFO_NAME);
            return EXIT_FAILURE;
        }

        const char *message =
            "Hello from Parent Process";

        printf("Parent sending: %s\n", message);

        if (write(fd, message, strlen(message) + 1) == -1)
        {
            perror("parent write");
            close(fd);
            unlink(FIFO_NAME);
            return EXIT_FAILURE;
        }

        close(fd);

        wait(NULL);

        printf("Parent: child process completed.\n");

        unlink(FIFO_NAME);
    }

    return EXIT_SUCCESS;
}



