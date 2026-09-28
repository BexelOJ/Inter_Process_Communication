#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define CHILD_COUNT 3

int main(void)
{
    int pipeFd[2];

    if (pipe(pipeFd) == -1)
    {
        perror("pipe");
        return 1;
    }

    for (int i = 0; i < CHILD_COUNT; i++)
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

            close(pipeFd[0]);

            snprintf(message,
                sizeof(message),
                "Message from child %d, PID=%d",
                i + 1,
                getpid());

            write(pipeFd[1],
                message,
                strlen(message) + 1);

            close(pipeFd[1]);

            return 0;
        }
    }

    /* Parent */

    close(pipeFd[1]);

    char buffer[100];

    for (int i = 0; i < CHILD_COUNT; i++)
    {
        ssize_t bytesRead;

        bytesRead = read(pipeFd[0],
            buffer,
            sizeof(buffer));

        if (bytesRead > 0)
        {
            printf("Parent received: %s\n", buffer);
        }
    }

    close(pipeFd[0]);

    for (int i = 0; i < CHILD_COUNT; i++)
    {
        wait(NULL);
    }

    return 0;
}


/*
//---------------------------------------------------
Multiple children communicate through the same pipe.

Architecture:

             ┌── Child 1 ──┐
             │              │
             ├── Child 2 ──┼──> [ PIPE ] ──> Parent
             │              │
             └── Child 3 ──┘

//---------------------------------------------------
*/


