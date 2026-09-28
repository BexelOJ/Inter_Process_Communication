#include <stdio.h>
#include <unistd.h>
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
        close(pipeFd[1]);

        printf("Child: calling read()...\n");
        printf("Child will block until data arrives.\n");

        read(pipeFd[0], buffer, sizeof(buffer));

        printf("Child: read() returned\n");
        printf("Child received data\n");

        close(pipeFd[0]);
    }
    else
    {
        close(pipeFd[0]);

        printf("Parent sleeping for 5 seconds...\n");

        sleep(5);

        printf("Parent writing data now\n");

        write(pipeFd[1], "DATA", 5);

        close(pipeFd[1]);

        wait(NULL);
    }

    return 0;
}


/*

The important observation:

Child:
read()
  ↓
BLOCKED
  ↓
Parent writes
  ↓
read() wakes up

*/



