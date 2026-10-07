#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t signalReceived = 0;

void signalHandler(int signal)
{
    signalReceived = signal;
}

int main(void)
{
    struct sigaction action;

    action.sa_handler = signalHandler;

    sigemptyset(&action.sa_mask);

    action.sa_flags = 0;

    sigaction(SIGUSR1,
        &action,
        NULL);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        printf("Child waiting for signal...\n");

        while (!signalReceived)
        {
            pause();
        }

        printf("Child received signal %d\n",
            signalReceived);

        _exit(EXIT_SUCCESS);
    }

    sleep(2);

    printf("Parent sending SIGUSR1...\n");

    kill(pid,
        SIGUSR1);

    waitpid(pid,
        NULL,
        0);

    printf("Child finished.\n");

    return EXIT_SUCCESS;
}



