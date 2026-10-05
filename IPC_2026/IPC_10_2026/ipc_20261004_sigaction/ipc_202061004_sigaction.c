#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signalHandler(int signalNumber)
{
    printf("Signal received: %d\n", signalNumber);
}

int main(void)
{
    struct sigaction action;

    action.sa_handler = signalHandler;

    sigemptyset(&action.sa_mask);

    action.sa_flags = 0;

    if (sigaction(SIGINT, &action, NULL) == -1)
    {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("Process running. Press Ctrl+C.\n");

    while (1)
    {
        pause();
    }

    return EXIT_SUCCESS;
}



