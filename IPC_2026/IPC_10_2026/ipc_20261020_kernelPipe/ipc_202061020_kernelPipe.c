#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    int pipefd[2];

    if (pipe(pipefd) < 0)
    {
        perror("pipe");
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
        /* Child = reader */

        close(pipefd[1]);

        char buffer[128];

        ssize_t bytes = read(
            pipefd[0],
            buffer,
            sizeof(buffer) - 1
        );

        if (bytes > 0)
        {
            buffer[bytes] = '\0';

            printf("Child received: %s\n", buffer);
        }

        close(pipefd[0]);

        return 0;
    }

    /* Parent = writer */

    close(pipefd[0]);

    const char* message =
        "Hello from parent through kernel pipe";

    write(
        pipefd[1],
        message,
        strlen(message)
    );

    close(pipefd[1]);

    waitpid(pid, NULL, 0);

    return 0;
}



