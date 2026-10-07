#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define QUEUE_NAME "/ipc_mq_benchmark"
#define ITERATIONS 10000

static long long time_ns(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (long long)ts.tv_sec * 1000000000LL +
        ts.tv_nsec;
}

int main(void)
{
    struct mq_attr attr = { 0 };

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1;

    mq_unlink(QUEUE_NAME);

    mqd_t mq = mq_open(
        QUEUE_NAME,
        O_CREAT | O_RDWR,
        0666,
        &attr
    );

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        char buffer;

        for (int i = 0; i < ITERATIONS; i++)
        {
            mq_receive(mq, &buffer, 1, NULL);
        }

        mq_close(mq);
        exit(0);
    }

    char data = 'X';

    long long start = time_ns();

    for (int i = 0; i < ITERATIONS; i++)
    {
        mq_send(mq, &data, 1, 0);
    }

    long long end = time_ns();

    waitpid(pid, NULL, 0);

    double total_us = (end - start) / 1000.0;

    printf("POSIX Message Queue Benchmark\n");
    printf("Messages : %d\n", ITERATIONS);
    printf("Total    : %.3f us\n", total_us);
    printf("Average  : %.6f us/message\n",
        total_us / ITERATIONS);

    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}



