#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void signalHandler(int signalNumber)
{
    if (signalNumber == SIGUSR1)
    {
        printf("Child: SIGUSR1 received.\n");
    }
}

int main(void)
{
    pid_t pid;

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        /* Child */

        signal(SIGUSR1, signalHandler);

        printf("Child PID: %d\n", getpid());
        printf("Child waiting for signal...\n");

        while (1)
        {
            pause();
        }
    }
    else
    {
        /* Parent */

        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);

        sleep(2);

        printf("Parent sending SIGUSR1...\n");

        if (kill(pid, SIGUSR1) == -1)
        {
            perror("kill");
            return EXIT_FAILURE;
        }

        sleep(2);

        printf("Parent terminating child...\n");

        kill(pid, SIGTERM);

        wait(NULL);

        printf("Child terminated.\n");
    }

    return EXIT_SUCCESS;
}



