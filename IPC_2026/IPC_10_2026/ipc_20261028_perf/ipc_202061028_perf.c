#include <stdio.h>
#include <unistd.h>

volatile unsigned long long result;

void cpu_work()
{
    for (unsigned long long i = 0;
        i < 500000000ULL;
        i++)
    {
        result += i;
    }
}

int main()
{
    printf(
        "PID: %d\n",
        getpid());

    printf(
        "Starting CPU workload...\n");

    cpu_work();

    printf(
        "Result = %llu\n",
        result);

    return 0;
}



