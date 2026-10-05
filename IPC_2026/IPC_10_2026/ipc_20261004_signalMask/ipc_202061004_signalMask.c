#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signalHandler(int signalNumber)
{
    printf("SIGUSR1 handler executed.\n");
}

int main(void)
{
    sigset_t signalSet;

    signal(SIGUSR1, signalHandler);

    sigemptyset(&signalSet);
    sigaddset(&signalSet, SIGUSR1);

    printf("Blocking SIGUSR1...\n");

    if (sigprocmask(SIG_BLOCK, &signalSet, NULL) == -1)
    {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    printf("PID: %d\n", getpid());
    printf("Send SIGUSR1 now.\n");

    sleep(10);

    printf("Unblocking SIGUSR1...\n");

    if (sigprocmask(SIG_UNBLOCK, &signalSet, NULL) == -1)
    {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    printf("SIGUSR1 unblocked.\n");

    return EXIT_SUCCESS;
}


// FROM ANOTHER TERMINAL

// kill -USR1 <PID>




