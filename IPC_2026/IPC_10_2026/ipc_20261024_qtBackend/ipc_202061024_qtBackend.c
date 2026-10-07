#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/qt_backend.sock"

int main(void)
{
    int server_fd;
    int client_fd;

    struct sockaddr_un address;

    char request[128];

    server_fd = socket(AF_UNIX,
        SOCK_STREAM,
        0);

    if (server_fd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    unlink(SOCKET_PATH);

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;

    strcpy(address.sun_path,
        SOCKET_PATH);

    if (bind(server_fd,
        (struct sockaddr*)&address,
        sizeof(address)) < 0)
    {
        perror("bind");

        close(server_fd);

        return EXIT_FAILURE;
    }

    listen(server_fd, 5);

    printf("Qt backend running\n");

    while (1)
    {
        client_fd =
            accept(server_fd,
                NULL,
                NULL);

        if (client_fd < 0)
            continue;

        memset(request, 0, sizeof(request));

        read(client_fd,
            request,
            sizeof(request) - 1);

        printf("Qt request: %s\n",
            request);

        const char* response =
            "{"
            "\"cpu\":42,"
            "\"ram\":63,"
            "\"disk\":51"
            "}";

        write(client_fd,
            response,
            strlen(response) + 1);

        close(client_fd);
    }

    close(server_fd);

    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}



