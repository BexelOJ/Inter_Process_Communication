#include <stdio.h>
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
        printf("Child started\n");
        printf("Child PID  : %d\n", getpid());
        printf("Initial PPID: %d\n", getppid());

        sleep(5);

        printf("\nAfter parent exits\n");
        printf("Child PID  : %d\n", getpid());
        printf("New PPID   : %d\n", getppid());

        return 0;
    }

    printf("Parent process\n");
    printf("Parent PID : %d\n", getpid());

    printf("Parent exiting immediately\n");

    return 0;
}


/*
//---------------------------------------------------
Before:

Parent
  |
  └── Child

Parent exits

//---------------------------------------------------
After:

init/system service
  |
  └── Child

//---------------------------------------------------
*/


