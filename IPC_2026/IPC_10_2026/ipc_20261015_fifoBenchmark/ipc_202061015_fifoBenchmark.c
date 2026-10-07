#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

#define FIFO_PATH "/tmp/ipc_benchmark_fifo"
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
    mkfifo(FIFO_PATH, 0666);

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        int fd = open(FIFO_PATH, O_RDONLY);

        if (fd == -1)
        {
            perror("open");
            exit(1);
        }

        char buffer;

        for (int i = 0; i < ITERATIONS; i++)
        {
            if (read(fd, &buffer, 1) != 1)
            {
                perror("read");
                break;
            }
        }

        close(fd);
        exit(0);
    }

    int fd = open(FIFO_PATH, O_WRONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    long long start = time_ns();

    for (int i = 0; i < ITERATIONS; i++)
    {
        char data = 'A';

        if (write(fd, &data, 1) != 1)
        {
            perror("write");
            break;
        }
    }

    long long end = time_ns();

    waitpid(pid, NULL, 0);

    double total_us = (end - start) / 1000.0;
    double avg_us = total_us / ITERATIONS;

    printf("FIFO Benchmark\n");
    printf("Iterations : %d\n", ITERATIONS);
    printf("Total      : %.3f us\n", total_us);
    printf("Average    : %.6f us\n", avg_us);

    close(fd);
    unlink(FIFO_PATH);

    return 0;
}



