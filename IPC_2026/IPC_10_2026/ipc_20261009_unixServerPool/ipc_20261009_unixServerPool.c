#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/unix_server_pool.sock"

void handleClient(int clientId)
{
    char buffer[256];

    memset(buffer, 0, sizeof(buffer));

    if (read(clientId,
        buffer,
        sizeof(buffer) - 1) > 0)
    {
        printf("Worker PID %d received: %s\n",
            getpid(),
            buffer);

        write(clientId,
            "Response from worker",
            strlen("Response from worker"));
    }

    close(clientId);

    exit(EXIT_SUCCESS);
}

int main(void)
{
    int serverId;
    int clientId;

    struct sockaddr_un address;

    serverId = socket(AF_UNIX, SOCK_STREAM, 0);

    if (serverId == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;

    snprintf(address.sun_path,
        sizeof(address.sun_path),
        "%s",
        SOCKET_PATH);

    unlink(SOCKET_PATH);

    if (bind(serverId,
        (struct sockaddr*)&address,
        sizeof(address)) == -1)
    {
        perror("bind");
        close(serverId);
        return EXIT_FAILURE;
    }

    if (listen(serverId, 10) == -1)
    {
        perror("listen");
        close(serverId);
        unlink(SOCKET_PATH);
        return EXIT_FAILURE;
    }

    printf("Unix server pool running.\n");

    while (1)
    {
        clientId = accept(serverId, NULL, NULL);

        if (clientId == -1)
        {
            perror("accept");
            continue;
        }

        pid_t processId = fork();

        if (processId == -1)
        {
            perror("fork");
            close(clientId);
            continue;
        }

        if (processId == 0)
        {
            /*
             * Child / worker
             */

            close(serverId);

            handleClient(clientId);
        }

        /*
         * Parent server
         */

        close(clientId);

        /*
         * Reap finished children.
         */

        while (waitpid(-1, NULL, WNOHANG) > 0)
        {
        }
    }

    close(serverId);

    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}



