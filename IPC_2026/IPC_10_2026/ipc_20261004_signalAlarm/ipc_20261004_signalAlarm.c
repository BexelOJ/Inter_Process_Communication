#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void alarmHandler(int signalNumber)
{
    printf("SIGALRM received: %d\n", signalNumber);
}

int main(void)
{
    signal(SIGALRM, alarmHandler);

    printf("Setting alarm for 5 seconds...\n");

    alarm(5);

    printf("Waiting for alarm...\n");

    pause();

    printf("Alarm completed.\n");

    return EXIT_SUCCESS;
}



