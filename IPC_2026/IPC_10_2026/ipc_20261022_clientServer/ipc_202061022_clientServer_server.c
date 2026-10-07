#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/ipc_client_server.sock"

int main(void)
{
    int server_fd;
    int client_fd;

    struct sockaddr_un address;

    char buffer[128];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    unlink(SOCKET_PATH);

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    if (bind(server_fd,
        (struct sockaddr*)&address,
        sizeof(address)) < 0)
    {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Server waiting...\n");

    client_fd = accept(server_fd, NULL, NULL);

    if (client_fd < 0)
    {
        perror("accept");
        close(server_fd);
        return EXIT_FAILURE;
    }

    read(client_fd, buffer, sizeof(buffer) - 1);

    printf("Server received: %s\n", buffer);

    strcpy(buffer, "Hello from server");

    write(client_fd, buffer, strlen(buffer) + 1);

    close(client_fd);
    close(server_fd);

    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}



