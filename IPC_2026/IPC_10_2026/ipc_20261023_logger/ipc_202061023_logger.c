#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    int pipe_fd[2];

    pipe(pipe_fd);

    pid_t pid = fork();

    if (pid == 0)
    {
        close(pipe_fd[1]);

        char message[128];

        while (read(pipe_fd[0],
            message,
            sizeof(message)) > 0)
        {
            printf("[LOGGER] %s\n",
                message);
        }

        close(pipe_fd[0]);

        exit(EXIT_SUCCESS);
    }

    close(pipe_fd[0]);

    const char* logs[] =
    {
        "System started",
        "Network connected",
        "Sensor initialized",
        "Data collection started",
        "System stopped"
    };

    for (int i = 0; i < 5; i++)
    {
        write(pipe_fd[1],
            logs[i],
            strlen(logs[i]) + 1);

        sleep(1);
    }

    close(pipe_fd[1]);

    wait(NULL);

    return EXIT_SUCCESS;
}



