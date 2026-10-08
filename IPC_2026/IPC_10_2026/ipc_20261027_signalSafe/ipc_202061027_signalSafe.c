#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t signal_received = 0;

void signal_handler(int signo)
{
    signal_received = 1;
}

int main()
{
    struct sigaction sa;

    sa.sa_handler = signal_handler;

    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;

    sigaction(
        SIGINT,
        &sa,
        NULL);

    printf(
        "Process running. Press Ctrl+C.\n");

    while (!signal_received)
    {
        printf(
            "Working...\n");

        sleep(1);
    }

    printf(
        "Signal received.\n");

    printf(
        "Performing cleanup in main context.\n");

    return 0;
}



