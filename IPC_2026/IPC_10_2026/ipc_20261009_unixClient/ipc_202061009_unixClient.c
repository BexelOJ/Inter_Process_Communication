#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/unix_server.sock"

int main(void)
{
    int socketId;

    struct sockaddr_un address;

    const char* message = "Hello from Unix client";

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
        (struct sockaddr*)&address,
        sizeof(address)) == -1)
    {
        perror("connect");
        close(socketId);
        return EXIT_FAILURE;
    }

    printf("Connected to Unix server.\n");

    if (write(socketId,
        message,
        strlen(message)) == -1)
    {
        perror("write");
        close(socketId);
        return EXIT_FAILURE;
    }

    printf("Message sent.\n");

    close(socketId);

    return EXIT_SUCCESS;
}



