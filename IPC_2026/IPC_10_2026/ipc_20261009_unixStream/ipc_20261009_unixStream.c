#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/unix_stream.sock"

int main(void)
{
    int socketId;
    int clientId;

    struct sockaddr_un address;

    char buffer[100];

    socketId = socket(AF_UNIX, SOCK_STREAM, 0);

    if (socketId == -1)
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

    if (bind(socketId,
        (struct sockaddr*)&address,
        sizeof(address)) == -1)
    {
        perror("bind");
        close(socketId);
        return EXIT_FAILURE;
    }

    if (listen(socketId, 5) == -1)
    {
        perror("listen");
        close(socketId);
        unlink(SOCKET_PATH);
        return EXIT_FAILURE;
    }

    printf("Unix stream server waiting...\n");

    clientId = accept(socketId, NULL, NULL);

    if (clientId == -1)
    {
        perror("accept");
        close(socketId);
        unlink(SOCKET_PATH);
        return EXIT_FAILURE;
    }

    memset(buffer, 0, sizeof(buffer));

    if (read(clientId, buffer, sizeof(buffer) - 1) == -1)
    {
        perror("read");
    }
    else
    {
        printf("Received: %s\n", buffer);
    }

    close(clientId);
    close(socketId);

    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}


