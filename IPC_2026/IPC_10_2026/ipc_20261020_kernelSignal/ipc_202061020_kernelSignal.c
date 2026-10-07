#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t signal_received = 0;

void signal_handler(int signal)
{
    signal_received = signal;
}

int main(void)
{
    struct sigaction action;

    action.sa_handler = signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(
        SIGUSR1,
        &action,
        NULL
    ) < 0)
    {
        perror("sigaction");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf(
            "Child PID %d waiting for signal...\n",
            getpid()
        );

        while (!signal_received)
            pause();

        printf(
            "Child received signal %d\n",
            signal_received
        );

        return 0;
    }

    sleep(1);

    printf(
        "Parent sending SIGUSR1 to child\n"
    );

    kill(pid, SIGUSR1);

    waitpid(pid, NULL, 0);

    return 0;
}



