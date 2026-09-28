#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signalHandler(int signalNumber)
{
    if (signalNumber == SIGUSR1)
    {
        printf("Received SIGUSR1\n");
    }
    else if (signalNumber == SIGUSR2)
    {
        printf("Received SIGUSR2\n");
    }
}

int main(void)
{
    printf("PID: %d\n", getpid());

    signal(SIGUSR1, signalHandler);
    signal(SIGUSR2, signalHandler);

    printf("Waiting for user signals...\n");

    while (1)
    {
        pause();
    }

    return 0;
}


/*

Demonstrates user-defined signals SIGUSR1 and SIGUSR2


From another terminal:

kill -USR1 <PID>
kill -USR2 <PID>


*/


