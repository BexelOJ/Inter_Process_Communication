#include <stdio.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    printf("Before fork()\n");
    printf("PID : %d\n", getpid());

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("\nChild process\n");
        printf("PID  : %d\n", getpid());
        printf("PPID : %d\n", getppid());
    }
    else
    {
        printf("\nParent process\n");
        printf("PID        : %d\n", getpid());
        printf("Child PID  : %d\n", pid);
    }

    return 0;
}


//---------------------------------------------------


