#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    int sockets[2];

    if (socketpair(
        AF_UNIX,
        SOCK_STREAM,
        0,
        sockets) < 0)
    {
        perror("socketpair");
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
        close(sockets[0]);

        char buffer[128];

        ssize_t bytes = read(
            sockets[1],
            buffer,
            sizeof(buffer) - 1
        );

        if (bytes > 0)
        {
            buffer[bytes] = '\0';

            printf("Child received: %s\n", buffer);
        }

        close(sockets[1]);

        return 0;
    }

    close(sockets[1]);

    const char* message =
        "Hello through kernel socket";

    write(
        sockets[0],
        message,
        strlen(message)
    );

    close(sockets[0]);

    waitpid(pid, NULL, 0);

    return 0;
}



