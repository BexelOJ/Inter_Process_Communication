#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signalHandler(int signalNumber)
{
    printf("Handler received signal: %d\n",
        signalNumber);
}

int main(void)
{
    sigset_t signalSet;

    signal(SIGUSR1, signalHandler);

    sigemptyset(&signalSet);
    sigaddset(&signalSet, SIGUSR1);

    printf("PID: %d\n", getpid());

    /*
     * Block SIGUSR1.
     */
    printf("Blocking SIGUSR1 for 10 seconds...\n");

    if (sigprocmask(SIG_BLOCK, &signalSet, NULL) == -1)
    {
        perror("sigprocmask");
        return 1;
    }

    sleep(10);

    printf("Unblocking SIGUSR1...\n");

    if (sigprocmask(SIG_UNBLOCK, &signalSet, NULL) == -1)
    {
        perror("sigprocmask");
        return 1;
    }

    printf("SIGUSR1 is now unblocked.\n");

    while (1)
    {
        pause();
    }

    return 0;
}


/*
 
Blocking and unblocking signals using a signal mask.


While it is sleeping:

kill -USR1 <PID>

The signal is blocked initially and becomes deliverable after SIG_UNBLOCK.


*/


