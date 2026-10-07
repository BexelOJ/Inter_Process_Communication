#include <stdio.h>
#include <time.h>

#define ITERATIONS 100000000UL

int main(void)
{
    volatile unsigned long result = 0;

    clock_t start = clock();

    for (unsigned long i = 0; i < ITERATIONS; i++)
    {
        result += i;
    }

    clock_t end = clock();

    double cpu_time =
        (double)(end - start) / CLOCKS_PER_SEC;

    printf("CPU Benchmark\n");
    printf("Iterations : %lu\n", ITERATIONS);
    printf("Result     : %lu\n", result);
    printf("CPU time   : %.6f seconds\n", cpu_time);

    return 0;
}



