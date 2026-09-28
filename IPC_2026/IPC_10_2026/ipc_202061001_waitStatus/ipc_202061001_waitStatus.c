#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child process\n");
        printf("PID : %d\n", getpid());

        exit(42);
    }

    printf("Parent waiting for child...\n");

    if (wait(&status) == -1)
    {
        perror("wait");
        return 1;
    }

    if (WIFEXITED(status))
    {
        printf("Child exited normally\n");
        printf("Exit status : %d\n", WEXITSTATUS(status));
    }

    return 0;
}


//---------------------------------------------------


