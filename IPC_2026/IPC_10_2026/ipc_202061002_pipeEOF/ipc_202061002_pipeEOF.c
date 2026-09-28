#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    int pipeFd[2];
    pid_t pid;
    char buffer[100];
    ssize_t bytesRead;

    if (pipe(pipeFd) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        close(pipeFd[1]);

        while ((bytesRead = read(pipeFd[0],
            buffer,
            sizeof(buffer) - 1)) > 0)
        {
            buffer[bytesRead] = '\0';

            printf("Child received: %s\n", buffer);
        }

        if (bytesRead == 0)
        {
            printf("Child received EOF\n");
        }

        close(pipeFd[0]);
    }
    else
    {
        close(pipeFd[0]);

        const char* message = "Message before EOF";

        write(pipeFd[1], message, strlen(message));

        /*
         * Closing the last write descriptor
         * causes the reader to receive EOF.
         */
        close(pipeFd[1]);

        wait(NULL);
    }

    return 0;
}


/*

Important:

write end open
      ↓
read() waits for data

write end closed
      ↓
no more writers
      ↓
read() returns 0
      ↓
EOF

*/


