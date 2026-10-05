#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/unix_basic.sock"

int main(void)
{
    int socketId;
    struct sockaddr_un address;

    socketId = socket(AF_UNIX, SOCK_STREAM, 0);

    if (socketId == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    printf("Unix socket created.\n");
    printf("Socket ID: %d\n", socketId);

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

    printf("Unix socket bound to: %s\n", SOCKET_PATH);

    close(socketId);

    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}


