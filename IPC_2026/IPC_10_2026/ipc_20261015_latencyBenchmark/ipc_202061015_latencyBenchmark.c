#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

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
    int parent_to_child[2];
    int child_to_parent[2];

    pipe(parent_to_child);
    pipe(child_to_parent);

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        char data;

        for (int i = 0; i < ITERATIONS; i++)
        {
            read(parent_to_child[0], &data, 1);
            write(child_to_parent[1], &data, 1);
        }

        close(parent_to_child[0]);
        close(child_to_parent[1]);

        exit(0);
    }

    close(parent_to_child[0]);
    close(child_to_parent[1]);

    char data = 'X';

    long long start = time_ns();

    for (int i = 0; i < ITERATIONS; i++)
    {
        write(parent_to_child[1], &data, 1);
        read(child_to_parent[0], &data, 1);
    }

    long long end = time_ns();

    double total_ns = (double)(end - start);
    double round_trip_ns = total_ns / ITERATIONS;
    double one_way_ns = round_trip_ns / 2.0;

    printf("Latency Benchmark\n");
    printf("Iterations      : %d\n", ITERATIONS);
    printf("Round trip      : %.2f ns\n", round_trip_ns);
    printf("Estimated one-way: %.2f ns\n", one_way_ns);

    close(parent_to_child[1]);
    close(child_to_parent[0]);

    waitpid(pid, NULL, 0);

    return 0;
}



