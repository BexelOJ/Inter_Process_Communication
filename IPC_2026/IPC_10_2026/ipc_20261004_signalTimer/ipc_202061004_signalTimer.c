#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void timerHandler(int signalNumber)
{
    printf("Timer signal received: %d\n",
        signalNumber);
}

int main(void)
{
    timer_t timerId;

    struct sigaction action;
    struct sigevent event;
    struct itimerspec timerSpec;

    action.sa_handler = timerHandler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGRTMIN, &action, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }

    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGRTMIN;
    event.sigev_value.sival_ptr = &timerId;

    if (timer_create(CLOCK_MONOTONIC,
        &event,
        &timerId) == -1)
    {
        perror("timer_create");
        return 1;
    }

    timerSpec.it_value.tv_sec = 2;
    timerSpec.it_value.tv_nsec = 0;

    /*
     * Repeat every 2 seconds.
     */
    timerSpec.it_interval.tv_sec = 2;
    timerSpec.it_interval.tv_nsec = 0;

    if (timer_settime(timerId,
        0,
        &timerSpec,
        NULL) == -1)
    {
        perror("timer_settime");
        return 1;
    }

    printf("PID: %d\n", getpid());
    printf("Timer running every 2 seconds...\n");

    while (1)
    {
        pause();
    }

    return 0;
}


/*

Uses POSIX timer APIs to generate a signal.


Here:

timer_create()
       ↓
timer_settime()
       ↓
2 seconds
       ↓
SIGRTMIN
       ↓
timerHandler()
       ↓
2 seconds
       ↓
SIGRTMIN
       ↓
...


*/


