#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/unix_client_server.sock"

int main(void)
{
    int socketId;

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

    if (connect(socketId,
                (struct sockaddr *)&address,
                sizeof(address)) == -1)
    {
        perror("connect");
        close(socketId);
        return EXIT_FAILURE;
    }

    write(socketId,
          "Hello from Unix client",
          strlen("Hello from Unix client"));

    memset(buffer, 0, sizeof(buffer));

    read(socketId, buffer, sizeof(buffer) - 1);

    printf("Server: %s\n", buffer);

    close(socketId);

    return EXIT_SUCCESS;
}



