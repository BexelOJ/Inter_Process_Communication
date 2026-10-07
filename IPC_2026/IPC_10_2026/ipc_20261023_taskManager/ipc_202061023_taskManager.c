#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void task(const char* name,
    int duration)
{
    printf("%s started PID=%d\n",
        name,
        getpid());

    sleep(duration);

    printf("%s completed\n",
        name);

    exit(EXIT_SUCCESS);
}

int main(void)
{
    const char* task_names[] =
    {
        "NetworkTask",
        "SensorTask",
        "DatabaseTask",
        "LoggerTask"
    };

    int durations[] =
    {
        2,
        3,
        4,
        1
    };

    pid_t pids[4];

    printf("Task Manager started\n");

    for (int i = 0; i < 4; i++)
    {
        pids[i] = fork();

        if (pids[i] == 0)
        {
            task(task_names[i],
                durations[i]);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        int status;

        waitpid(pids[i],
            &status,
            0);

        if (WIFEXITED(status))
        {
            printf("Task Manager: %s completed, status=%d\n",
                task_names[i],
                WEXITSTATUS(status));
        }
    }

    printf("All tasks completed\n");

    return EXIT_SUCCESS;
}



