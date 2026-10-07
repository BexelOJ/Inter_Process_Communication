#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define SIZE (100 * 1024 * 1024)

int main(void)
{
    char* shared = mmap(
        NULL,
        SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );

    if (shared == MAP_FAILED)
    {
        perror("mmap");
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
        volatile char value = shared[SIZE - 1];

        (void)value;

        _exit(0);
    }

    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    memset(shared, 'X', SIZE);

    clock_gettime(CLOCK_MONOTONIC, &end);

    waitpid(pid, NULL, 0);

    double seconds =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    double bandwidth =
        ((double)SIZE /
            (1024.0 * 1024.0 * 1024.0))
        / seconds;

    printf("Shared Memory Benchmark\n");
    printf("Size      : %d MB\n",
        SIZE / (1024 * 1024));
    printf("Time      : %.6f seconds\n", seconds);
    printf("Bandwidth : %.2f GB/s\n", bandwidth);

    munmap(shared, SIZE);

    return 0;
}



