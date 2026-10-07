#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE (256 * 1024 * 1024)

static double elapsed_seconds(struct timespec* start,
    struct timespec* end)
{
    return (end->tv_sec - start->tv_sec) +
        (end->tv_nsec - start->tv_nsec) / 1e9;
}

int main(void)
{
    char* src = malloc(SIZE);
    char* dst = malloc(SIZE);

    if (src == NULL || dst == NULL)
    {
        perror("malloc");
        return 1;
    }

    memset(src, 'A', SIZE);

    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    memcpy(dst, src, SIZE);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double seconds = elapsed_seconds(&start, &end);

    double bandwidth =
        ((double)SIZE / (1024.0 * 1024.0 * 1024.0))
        / seconds;

    printf("Memory Benchmark\n");
    printf("Size      : %d MB\n", SIZE / (1024 * 1024));
    printf("Time      : %.6f seconds\n", seconds);
    printf("Bandwidth : %.2f GB/s\n", bandwidth);

    free(src);
    free(dst);

    return 0;
}



