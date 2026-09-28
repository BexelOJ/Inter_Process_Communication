#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    int pipeFd[2];
    pid_t pid;
    char buffer[100];

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
        /* Child writes */
        const char* message = "Hello from child";

        close(pipeFd[0]);

        write(pipeFd[1], message, strlen(message) + 1);

        close(pipeFd[1]);
    }
    else
    {
        /* Parent reads */
        close(pipeFd[1]);

        read(pipeFd[0], buffer, sizeof(buffer));

        printf("Parent received: %s\n", buffer);

        close(pipeFd[0]);

        wait(NULL);
    }

    return 0;
}


/*
 
Flow:

Child
  |
  | write()
  v
[ PIPE ]
  |
  | read()
  v
Parent

*/



