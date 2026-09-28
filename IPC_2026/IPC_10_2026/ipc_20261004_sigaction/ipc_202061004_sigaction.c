#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signalHandler(int signalNumber)
{
    printf("sigaction handler received signal: %d\n",
        signalNumber);
}

int main(void)
{
    struct sigaction action;

    action.sa_handler = signalHandler;

    sigemptyset(&action.sa_mask);

    action.sa_flags = 0;

    if (sigaction(SIGUSR1, &action, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }

    printf("PID: %d\n", getpid());
    printf("Waiting for SIGUSR1...\n");

    while (1)
    {
        pause();
    }

    return 0;
}


/*

Modern POSIX-style signal handling using sigaction()


The important structure is:

struct sigaction
        |
        +-- sa_handler
        +-- sa_mask
        +-- sa_flags


*/


