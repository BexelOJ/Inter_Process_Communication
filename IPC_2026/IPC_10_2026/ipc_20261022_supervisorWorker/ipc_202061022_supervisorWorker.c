#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void worker(int id)
{
    printf("Worker %d started PID=%d\n",
        id,
        getpid());

    sleep(2);

    printf("Worker %d finished\n", id);

    exit(EXIT_SUCCESS);
}

int main(void)
{
    const int worker_count = 3;

    pid_t workers[worker_count];

    printf("Supervisor started PID=%d\n",
        getpid());

    for (int i = 0; i < worker_count; i++)
    {
        workers[i] = fork();

        if (workers[i] == 0)
        {
            worker(i + 1);
        }
    }

    for (int i = 0; i < worker_count; i++)
    {
        int status;

        waitpid(workers[i],
            &status,
            0);

        if (WIFEXITED(status))
        {
            printf("Supervisor: Worker %d exited with %d\n",
                i + 1,
                WEXITSTATUS(status));
        }
    }

    printf("Supervisor: all workers completed\n");

    return EXIT_SUCCESS;
}



