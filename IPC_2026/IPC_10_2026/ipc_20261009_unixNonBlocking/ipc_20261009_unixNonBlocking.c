#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/unix_nonblocking.sock"

int main(void)
{
    int serverId;
    int clientId;

    struct sockaddr_un address;

    char buffer[100];

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

    /*
     * Make listening socket non-blocking.
     */

    if (fcntl(serverId, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("fcntl");
        close(serverId);
        unlink(SOCKET_PATH);
        return EXIT_FAILURE;
    }

    printf("Non-blocking Unix server started.\n");

    clientId = accept(serverId, NULL, NULL);

    if (clientId == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            printf("No client available right now.\n");
        }
        else
        {
            perror("accept");
        }

        close(serverId);
        unlink(SOCKET_PATH);

        return EXIT_SUCCESS;
    }

    memset(buffer, 0, sizeof(buffer));

    read(clientId, buffer, sizeof(buffer) - 1);

    printf("Received: %s\n", buffer);

    close(clientId);
    close(serverId);

    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}


