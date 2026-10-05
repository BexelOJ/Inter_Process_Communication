#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(void)
{
    sigset_t signalSet;
    int receivedSignal;

    sigemptyset(&signalSet);

    sigaddset(&signalSet, SIGUSR1);

    /*
     * Block SIGUSR1.
     *
     * sigwait() will receive it synchronously.
     */
    if (sigprocmask(SIG_BLOCK, &signalSet, NULL) == -1)
    {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    printf("PID: %d\n", getpid());
    printf("Waiting for SIGUSR1...\n");

    if (sigwait(&signalSet, &receivedSignal) != 0)
    {
        perror("sigwait");
        return EXIT_FAILURE;
    }

    printf("Received signal: %d\n", receivedSignal);

    return EXIT_SUCCESS;
}


// FROM ANOTHER TERMINAL

// kill -USR1 <PID>


