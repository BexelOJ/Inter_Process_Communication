#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void alarmHandler(int signalNumber)
{
    printf("SIGALRM received: %d\n",
        signalNumber);

    printf("Alarm expired.\n");
}

int main(void)
{
    signal(SIGALRM, alarmHandler);

    printf("PID: %d\n", getpid());

    printf("Setting alarm for 5 seconds...\n");

    alarm(5);

    printf("Waiting...\n");

    while (1)
    {
        pause();
    }

    return 0;
}


/*

Uses alarm() to generate SIGALRM.


Flow:

alarm(5)
   |
   | 5 seconds
   v
SIGALRM
   |
   v
alarmHandler()


*/


