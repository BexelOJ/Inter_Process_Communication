#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/request_response.sock"

int main(void)
{
    int server_fd;
    int client_fd;

    struct sockaddr_un address;

    char request[128];
    char response[128];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    unlink(SOCKET_PATH);

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    bind(server_fd,
        (struct sockaddr*)&address,
        sizeof(address));

    listen(server_fd, 5);

    printf("Request/Response server waiting...\n");

    client_fd = accept(server_fd, NULL, NULL);

    read(client_fd, request, sizeof(request));

    printf("Request: %s\n", request);

    snprintf(response,
        sizeof(response),
        "Response generated for: %s",
        request);

    write(client_fd,
        response,
        strlen(response) + 1);

    close(client_fd);
    close(server_fd);

    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}



