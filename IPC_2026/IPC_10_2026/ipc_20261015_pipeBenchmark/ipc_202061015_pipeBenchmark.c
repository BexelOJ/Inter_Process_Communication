#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define TOTAL_SIZE (100 * 1024 * 1024)
#define BUFFER_SIZE (64 * 1024)

static double elapsed(struct timespec* start,
    struct timespec* end)
{
    return (end->tv_sec - start->tv_sec) +
        (end->tv_nsec - start->tv_nsec) / 1e9;
}

int main(void)
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
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
        close(pipefd[1]);

        char buffer[BUFFER_SIZE];
        size_t received = 0;

        while (received < TOTAL_SIZE)
        {
            ssize_t n = read(pipefd[0],
                buffer,
                sizeof(buffer));

            if (n <= 0)
                break;

            received += n;
        }

        close(pipefd[0]);
        exit(0);
    }

    close(pipefd[0]);

    char* buffer = malloc(BUFFER_SIZE);

    if (buffer == NULL)
    {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = 'A';

    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    size_t sent = 0;

    while (sent < TOTAL_SIZE)
    {
        size_t remaining = TOTAL_SIZE - sent;
        size_t size =
            remaining < BUFFER_SIZE ?
            remaining : BUFFER_SIZE;

        ssize_t n = write(pipefd[1], buffer, size);

        if (n <= 0)
            break;

        sent += n;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    waitpid(pid, NULL, 0);

    double seconds = elapsed(&start, &end);

    double mbps =
        ((double)sent / (1024.0 * 1024.0))
        / seconds;

    printf("Pipe Benchmark\n");
    printf("Transferred : %.2f MB\n",
        (double)sent / (1024 * 1024));
    printf("Time        : %.6f seconds\n", seconds);
    printf("Throughput  : %.2f MB/s\n", mbps);

    free(buffer);
    close(pipefd[1]);

    return 0;
}



