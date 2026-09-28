#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    int parentToChild[2];
    int childToParent[2];

    pid_t pid;

    char buffer[100];

    if (pipe(parentToChild) == -1)
    {
        perror("pipe");
        return 1;
    }

    if (pipe(childToParent) == -1)
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
        /* Child */

        close(parentToChild[1]);
        close(childToParent[0]);

        read(parentToChild[0], buffer, sizeof(buffer));

        printf("Child received: %s\n", buffer);

        const char* reply = "Hello from child";

        write(childToParent[1],
            reply,
            strlen(reply) + 1);

        close(parentToChild[0]);
        close(childToParent[1]);
    }
    else
    {
        /* Parent */

        close(parentToChild[0]);
        close(childToParent[1]);

        const char* message = "Hello from parent";

        write(parentToChild[1],
            message,
            strlen(message) + 1);

        read(childToParent[0],
            buffer,
            sizeof(buffer));

        printf("Parent received: %s\n", buffer);

        close(parentToChild[1]);
        close(childToParent[0]);

        wait(NULL);
    }

    return 0;
}

/*
* 
Architecture:

             PIPE 1
Parent ------------------> Child
       parentToChild


             PIPE 2
Parent <------------------ Child
       childToParent

*/


