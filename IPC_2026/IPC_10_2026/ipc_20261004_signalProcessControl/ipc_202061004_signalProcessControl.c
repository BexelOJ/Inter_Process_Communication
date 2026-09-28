#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

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
        /*
         * Child process.
         */
        printf("Child started\n");
        printf("Child PID: %d\n", getpid());

        while (1)
        {
            printf("Child is running...\n");
            sleep(1);
        }
    }

    /*
     * Parent.
     */
    printf("Parent PID: %d\n", getpid());
    printf("Child PID : %d\n", pid);

    sleep(3);

    printf("\nParent sending SIGSTOP\n");

    kill(pid, SIGSTOP);

    sleep(3);

    printf("Parent sending SIGCONT\n");

    kill(pid, SIGCONT);

    sleep(3);

    printf("Parent sending SIGTERM\n");

    kill(pid, SIGTERM);

    waitpid(pid, NULL, 0);

    printf("Child terminated.\n");

    return 0;
}


/*
* 
Uses signals to control another process.
The parent starts a child. The parent then sends:

SIGSTOP
SIGCONT
SIGTERM

to the child.


This demonstrates signals as a process-control mechanism:

Parent
  |
  +---- SIGSTOP ---> Child
  |
  +---- SIGCONT ---> Child
  |
  +---- SIGTERM ---> Child


*/


