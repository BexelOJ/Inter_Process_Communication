#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/unix_chat.sock"

int main(void)
{
    int serverId;
    int clientId;

    struct sockaddr_un address;

    char buffer[256];

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

    if (listen(serverId, 5) == -1)
    {
        perror("listen");
        close(serverId);
        unlink(SOCKET_PATH);
        return EXIT_FAILURE;
    }

    printf("Chat server waiting...\n");

    clientId = accept(serverId, NULL, NULL);

    if (clientId == -1)
    {
        perror("accept");
        close(serverId);
        unlink(SOCKET_PATH);
        return EXIT_FAILURE;
    }

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        if (read(clientId,
            buffer,
            sizeof(buffer) - 1) <= 0)
        {
            break;
        }

        printf("Client: %s\n", buffer);

        if (strcmp(buffer, "exit\n") == 0)
        {
            break;
        }

        printf("Server: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            break;
        }

        write(clientId,
            buffer,
            strlen(buffer));

        if (strcmp(buffer, "exit\n") == 0)
        {
            break;
        }
    }

    close(clientId);
    close(serverId);

    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}



