#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void timerHandler(int signalNumber)
{
    printf("Timer signal received: %d\n", signalNumber);
}

int main(void)
{
    timer_t timerId;
    struct sigevent event;
    struct itimerspec timerSpec;

    signal(SIGUSR1, timerHandler);

    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGUSR1;
    event.sigev_value.sival_ptr = &timerId;

    if (timer_create(CLOCK_REALTIME, &event, &timerId) == -1)
    {
        perror("timer_create");
        return EXIT_FAILURE;
    }

    timerSpec.it_value.tv_sec = 2;
    timerSpec.it_value.tv_nsec = 0;

    timerSpec.it_interval.tv_sec = 2;
    timerSpec.it_interval.tv_nsec = 0;

    if (timer_settime(timerId, 0, &timerSpec, NULL) == -1)
    {
        perror("timer_settime");
        return EXIT_FAILURE;
    }

    printf("Timer started.\n");
    printf("Signal every 2 seconds.\n");

    while (1)
    {
        pause();
    }

    return EXIT_SUCCESS;
}



