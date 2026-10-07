#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define WORKER_COUNT 3
#define JOB_COUNT    9

void worker(int id)
{
    while (1)
    {
        int job;

        if (scanf("%d", &job) != 1)
            break;

        printf("Worker %d processing job %d\n",
            id,
            job);

        sleep(1);

        printf("Worker %d completed job %d\n",
            id,
            job);
    }

    exit(EXIT_SUCCESS);
}

int main(void)
{
    pid_t workers[WORKER_COUNT];

    for (int i = 0; i < WORKER_COUNT; i++)
    {
        workers[i] = fork();

        if (workers[i] == 0)
        {
            worker(i + 1);
        }
    }

    /*
     * In a real worker pool, jobs would normally
     * arrive through a shared queue, pipe,
     * message queue, socket, etc.
     */

    for (int job = 1; job <= JOB_COUNT; job++)
    {
        printf("Submitting job %d\n", job);
    }

    for (int i = 0; i < WORKER_COUNT; i++)
    {
        waitpid(workers[i], NULL, 0);
    }

    return EXIT_SUCCESS;
}



