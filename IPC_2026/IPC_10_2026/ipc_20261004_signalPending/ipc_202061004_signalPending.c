#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(void)
{
    sigset_t signalSet;
    sigset_t pendingSet;

    sigemptyset(&signalSet);
    sigaddset(&signalSet, SIGUSR1);

    printf("Blocking SIGUSR1...\n");

    if (sigprocmask(SIG_BLOCK, &signalSet, NULL) == -1)
    {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    printf("PID: %d\n", getpid());
    printf("Send SIGUSR1 from another terminal.\n");

    sleep(10);

    if (sigpending(&pendingSet) == -1)
    {
        perror("sigpending");
        return EXIT_FAILURE;
    }

    if (sigismember(&pendingSet, SIGUSR1))
    {
        printf("SIGUSR1 is pending.\n");
    }
    else
    {
        printf("SIGUSR1 is not pending.\n");
    }

    return EXIT_SUCCESS;
}



