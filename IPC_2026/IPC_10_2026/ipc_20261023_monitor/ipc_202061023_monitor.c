#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t worker;

    worker = fork();

    if (worker == 0)
    {
        printf("Worker started: PID=%d\n",
            getpid());

        for (int i = 1; i <= 5; i++)
        {
            printf("Worker running: %d\n", i);

            sleep(1);
        }

        printf("Worker exiting\n");

        exit(0);
    }

    printf("Monitor started\n");

    while (1)
    {
        int status;

        pid_t result =
            waitpid(worker,
                &status,
                WNOHANG);

        if (result == 0)
        {
            printf("Monitor: worker still running\n");

            sleep(1);
        }
        else if (result == worker)
        {
            if (WIFEXITED(status))
            {
                printf("Monitor: worker exited with status %d\n",
                    WEXITSTATUS(status));
            }

            break;
        }
        else
        {
            perror("waitpid");
            break;
        }
    }

    printf("Monitor stopped\n");

    return EXIT_SUCCESS;
}



