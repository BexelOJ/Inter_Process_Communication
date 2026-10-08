#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>

int main()
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
        close(pipefd[0]);

        sleep(2);

        const char* message =
            "Async pipe message";

        write(
            pipefd[1],
            message,
            strlen(message) + 1);

        close(pipefd[1]);

        return 0;
    }

    close(pipefd[1]);

    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(pipefd[0], &readfds);

    printf("Parent: waiting asynchronously...\n");

    int result = select(
        pipefd[0] + 1,
        &readfds,
        NULL,
        NULL,
        NULL);

    if (result > 0 &&
        FD_ISSET(pipefd[0], &readfds))
    {
        char buffer[256];

        memset(buffer, 0, sizeof(buffer));

        read(
            pipefd[0],
            buffer,
            sizeof(buffer) - 1);

        printf(
            "Parent received: %s\n",
            buffer);
    }

    close(pipefd[0]);

    return 0;
}



