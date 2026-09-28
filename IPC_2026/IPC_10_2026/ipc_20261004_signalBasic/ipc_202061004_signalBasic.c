#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
    printf("Process PID: %d\n", getpid());

    printf("Sending SIGUSR1 to myself...\n");

    kill(getpid(), SIGUSR1);

    printf("This line will not execute with default SIGUSR1 action.\n");

    return 0;
}


/*

Basic kill() + SIGUSR1

SIGUSR1 has the default action of terminating the process.


*/


