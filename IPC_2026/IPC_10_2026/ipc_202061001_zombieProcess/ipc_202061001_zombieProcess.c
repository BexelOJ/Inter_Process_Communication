#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child process exiting\n");
        printf("Child PID : %d\n", getpid());

        exit(0);
    }

    printf("Parent process\n");
    printf("Parent PID : %d\n", getpid());
    printf("Child PID  : %d\n", pid);

    printf("Parent will sleep for 30 seconds.\n");
    printf("Run: ps -el | grep %d\n", pid);

    sleep(30);

    return 0;
}


//---------------------------------------------------


