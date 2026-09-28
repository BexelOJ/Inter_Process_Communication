#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signalHandler(int signalNumber)
{
    printf("Signal received: %d\n", signalNumber);
}

int main(void)
{
    printf("PID: %d\n", getpid());

    signal(SIGUSR1, signalHandler);

    printf("Waiting for SIGUSR1...\n");
    printf("Run from another terminal:\n");
    printf("kill -USR1 %d\n", getpid());

    while (1)
    {
        pause();
    }

    return 0;
}


/*

Register a signal handler using signal()


From another terminal:

kill -USR1 <PID>


*/


