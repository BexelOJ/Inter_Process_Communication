#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t startWorker(void)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        printf("Worker started. PID=%d\n",
            getpid());

        /*
         * Simulate worker.
         */
        sleep(5);

        printf("Worker exiting. PID=%d\n",
            getpid());

        exit(1);
    }

    return pid;
}

int main(void)
{
    printf("Supervisor PID=%d\n",
        getpid());

    while (1)
    {
        pid_t workerPid = startWorker();

        printf("Supervisor: monitoring PID=%d\n",
            workerPid);

        int status;

        waitpid(workerPid,
            &status,
            0);

        if (WIFEXITED(status))
        {
            printf("Worker exited with status %d\n",
                WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Worker killed by signal %d\n",
                WTERMSIG(status));
        }

        printf("Restarting worker...\n");

        sleep(1);
    }

    return EXIT_SUCCESS;
}



