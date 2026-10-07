i
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 6000

int main()
{
    int server_fd;
    int client_fd;

    struct sockaddr_in addr;

    char buffer[1024];

    server_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr =
        INADDR_ANY;
    addr.sin_port =
        htons(PORT);

    bind(
        server_fd,
        (struct sockaddr*)&addr,
        sizeof(addr));

    listen(
        server_fd,
        5);

    printf(
        "Service B running...\n");

    while (1)
    {
        client_fd = accept(
            server_fd,
            NULL,
            NULL);

        memset(
            buffer,
            0,
            sizeof(buffer));

        read(
            client_fd,
            buffer,
            sizeof(buffer) - 1);

        printf(
            "Service B received: %s\n",
            buffer);

        const char* response =
            "processed by Service B";

        write(
            client_fd,
            response,
            strlen(response));

        close(client_fd);
    }

    return 0;
}



