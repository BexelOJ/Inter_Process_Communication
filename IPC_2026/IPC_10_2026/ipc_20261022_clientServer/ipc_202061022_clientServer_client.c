#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/ipc_client_server.sock"

int main(void)
{
    int client_fd;

    struct sockaddr_un address;

    char buffer[128];

    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (client_fd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    if (connect(client_fd,
                (struct sockaddr *)&address,
                sizeof(address)) < 0)
    {
        perror("connect");
        close(client_fd);
        return EXIT_FAILURE;
    }

    strcpy(buffer, "Hello from client");

    write(client_fd, buffer, strlen(buffer) + 1);

    read(client_fd, buffer, sizeof(buffer) - 1);

    printf("Client received: %s\n", buffer);

    close(client_fd);

    return EXIT_SUCCESS;
}




