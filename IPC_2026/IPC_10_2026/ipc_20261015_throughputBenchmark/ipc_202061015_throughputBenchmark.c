#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_BYTES (500 * 1024 * 1024)

int main(void)
{
    char* buffer = malloc(TOTAL_BYTES);

    if (buffer == NULL)
    {
        perror("malloc");
        return 1;
    }

    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t i = 0; i < TOTAL_BYTES; i++)
    {
        buffer[i] = (char)(i & 0xFF);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double seconds =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    double mb =
        (double)TOTAL_BYTES /
        (1024.0 * 1024.0);

    double mbps = mb / seconds;

    printf("Throughput Benchmark\n");
    printf("Data       : %.2f MB\n", mb);
    printf("Time       : %.6f seconds\n", seconds);
    printf("Throughput : %.2f MB/s\n", mbps);

    free(buffer);

    return 0;
}



