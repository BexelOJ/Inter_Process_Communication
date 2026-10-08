// ---------------------------------------------------
// ipc_20261029_monitoringSystem
// Shared-memory monitoring system
// ---------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>

typedef struct
{
    int cpu;
    int ram;
    int temperature;
} Metrics;

int main(void)
{
    Metrics* metrics =
        mmap(NULL,
            sizeof(Metrics),
            PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS,
            -1,
            0);

    if (metrics == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        srand(time(NULL) ^ getpid());

        while (1)
        {
            metrics->cpu =
                rand() % 101;

            metrics->ram =
                30 + rand() % 60;

            metrics->temperature =
                30 + rand() % 50;

            sleep(1);
        }
    }

    for (int i = 0; i < 10; i++)
    {
        printf("CPU : %d%%\n",
            metrics->cpu);

        printf("RAM : %d%%\n",
            metrics->ram);

        printf("TEMP: %d C\n\n",
            metrics->temperature);

        sleep(1);
    }

    kill(pid, SIGTERM);

    waitpid(pid, NULL, 0);

    munmap(metrics, sizeof(Metrics));

    return 0;
}



