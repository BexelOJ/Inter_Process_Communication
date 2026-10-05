#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signalHandler(int signalNumber)
{
    if (signalNumber == SIGINT)
    {
        printf("SIGINT received.\n");
    }
    else if (signalNumber == SIGTERM)
    {
        printf("SIGTERM received.\n");
    }
    else if (signalNumber == SIGUSR1)
    {
        printf("SIGUSR1 received.\n");
    }
}

int main(void)
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGUSR1, signalHandler);

    printf("PID: %d\n", getpid());
    printf("Waiting for signals...\n");

    while (1)
    {
        pause();
    }

    return EXIT_SUCCESS;
}


// FROM ANOTHER TERMINAL:

// kill -USR1 <PID>
// or
// kill -TERM <PID>


