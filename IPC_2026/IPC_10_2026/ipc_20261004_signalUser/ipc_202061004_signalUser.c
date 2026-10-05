#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signalHandler(int signalNumber)
{
    if (signalNumber == SIGUSR1)
    {
        printf("SIGUSR1 received.\n");
    }
    else if (signalNumber == SIGUSR2)
    {
        printf("SIGUSR2 received.\n");
    }
}

int main(void)
{
    signal(SIGUSR1, signalHandler);
    signal(SIGUSR2, signalHandler);

    printf("PID: %d\n", getpid());
    printf("Waiting for user-defined signals...\n");

    while (1)
    {
        pause();
    }

    return EXIT_SUCCESS;
}


// FROM ANOTHER TERMINAL

// kill -USR1 <PID>
// and 
// kill -USR2 <PID>
