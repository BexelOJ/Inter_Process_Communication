#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
    sigset_t signalSet;
    sigset_t pendingSet;

    printf("PID: %d\n", getpid());

    sigemptyset(&signalSet);
    sigaddset(&signalSet, SIGUSR1);

    /*
     * Block SIGUSR1.
     */
    if (sigprocmask(SIG_BLOCK, &signalSet, NULL) == -1)
    {
        perror("sigprocmask");
        return 1;
    }

    printf("SIGUSR1 is blocked.\n");
    printf("Send SIGUSR1 now:\n");
    printf("kill -USR1 %d\n", getpid());

    sleep(10);

    /*
     * Get pending signals.
     */
    if (sigpending(&pendingSet) == -1)
    {
        perror("sigpending");
        return 1;
    }

    if (sigismember(&pendingSet, SIGUSR1))
    {
        printf("SIGUSR1 is pending.\n");
    }
    else
    {
        printf("SIGUSR1 is not pending.\n");
    }

    /*
     * Unblock SIGUSR1.
     */
    printf("Unblocking SIGUSR1...\n");

    if (sigprocmask(SIG_UNBLOCK, &signalSet, NULL) == -1)
    {
        perror("sigprocmask");
        return 1;
    }

    return 0;
}


/*

Checks whether a blocked signal is pending.


Concept:

SIGUSR1
   |
   | blocked
   v
pending
   |
   | unblock
   v
delivered


*/


