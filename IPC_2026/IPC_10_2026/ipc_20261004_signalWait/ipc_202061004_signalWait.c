#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
    sigset_t signalSet;
    int signalNumber;

    sigemptyset(&signalSet);

    sigaddset(&signalSet, SIGUSR1);
    sigaddset(&signalSet, SIGUSR2);

    /*
     * Block these signals.
     */
    if (sigprocmask(SIG_BLOCK, &signalSet, NULL) == -1)
    {
        perror("sigprocmask");
        return 1;
    }

    printf("PID: %d\n", getpid());
    printf("Waiting synchronously for SIGUSR1/SIGUSR2...\n");

    while (1)
    {
        if (sigwait(&signalSet, &signalNumber) != 0)
        {
            perror("sigwait");
            return 1;
        }

        printf("sigwait received signal: %d\n",
            signalNumber);
    }

    return 0;
}


/*

Wait synchronously for a signal using sigwait()


Unlike a normal signal handler:

Asynchronous:

signal
  ↓
handler()


Here:

sigwait()
   ↓
BLOCK
   ↓
signal arrives
   ↓
sigwait() returns

*/


