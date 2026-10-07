#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(void)
{
    pid_t worker;

    worker = fork();

    if (worker == 0)
    {
        printf("Managed process started PID=%d\n",
            getpid());

        while (1)
        {
            printf("Managed process running...\n");

            sleep(1);
        }
    }

    printf("Process Manager\n");
    printf("Worker PID = %d\n", worker);

    sleep(5);

    printf("Manager: stopping worker\n");

    kill(worker, SIGTERM);

    waitpid(worker, NULL, 0);

    printf("Manager: worker terminated\n");

    return EXIT_SUCCESS;
}



