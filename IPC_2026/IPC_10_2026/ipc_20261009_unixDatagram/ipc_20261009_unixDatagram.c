#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH "/tmp/unix_dgram_server.sock"
#define CLIENT_PATH "/tmp/unix_dgram_client.sock"

int main(void)
{
    int socketId;

    struct sockaddr_un serverAddress;
    struct sockaddr_un clientAddress;

    const char* message = "Hello using Unix Datagram";

    char buffer[100];

    socketId = socket(AF_UNIX, SOCK_DGRAM, 0);

    if (socketId == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    memset(&clientAddress, 0, sizeof(clientAddress));

    clientAddress.sun_family = AF_UNIX;

    snprintf(clientAddress.sun_path,
        sizeof(clientAddress.sun_path),
        "%s",
        CLIENT_PATH);

    unlink(CLIENT_PATH);

    if (bind(socketId,
        (struct sockaddr*)&clientAddress,
        sizeof(clientAddress)) == -1)
    {
        perror("bind");
        close(socketId);
        return EXIT_FAILURE;
    }

    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sun_family = AF_UNIX;

    snprintf(serverAddress.sun_path,
        sizeof(serverAddress.sun_path),
        "%s",
        SERVER_PATH);

    printf("Sending datagram...\n");

    if (sendto(socketId,
        message,
        strlen(message),
        0,
        (struct sockaddr*)&serverAddress,
        sizeof(serverAddress)) == -1)
    {
        perror("sendto");
        close(socketId);
        unlink(CLIENT_PATH);
        return EXIT_FAILURE;
    }

    printf("Datagram sent.\n");

    /*
     * This example expects a receiver to exist at SERVER_PATH.
     */

    memset(buffer, 0, sizeof(buffer));

    close(socketId);

    unlink(CLIENT_PATH);

    return EXIT_SUCCESS;
}


