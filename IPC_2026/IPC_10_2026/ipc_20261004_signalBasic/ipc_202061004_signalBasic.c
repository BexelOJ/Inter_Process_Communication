#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signalHandler(int signalNumber)
{
    printf("Signal received: %d\n", signalNumber);
}

int main(void)
{
    signal(SIGUSR1, signalHandler);

    printf("Raising SIGUSR1...\n");

    raise(SIGUSR1);

    printf("Program continues after signal.\n");

    return EXIT_SUCCESS;
}



