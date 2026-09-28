#include <stdio.h>
#include <unistd.h>

int main(void)
{
    pid_t child1;
    pid_t child2;

    printf("Root process\n");
    printf("PID : %d\n\n", getpid());

    child1 = fork();

    if (child1 < 0)
    {
        perror("fork");
        return 1;
    }

    if (child1 == 0)
    {
        printf("Child 1\n");
        printf("PID  : %d\n", getpid());
        printf("PPID : %d\n", getppid());

        return 0;
    }

    child2 = fork();

    if (child2 < 0)
    {
        perror("fork");
        return 1;
    }

    if (child2 == 0)
    {
        printf("Child 2\n");
        printf("PID  : %d\n", getpid());
        printf("PPID : %d\n", getppid());

        return 0;
    }

    printf("\nParent\n");
    printf("PID       : %d\n", getpid());
    printf("Child 1   : %d\n", child1);
    printf("Child 2   : %d\n", child2);

    return 0;
}


//---------------------------------------------------


