#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int server_fd;
    int client_fd;

    struct sockaddr_in addr;

    char buffer[4096];

    server_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(
        server_fd,
        (struct sockaddr*)&addr,
        sizeof(addr));

    listen(server_fd, 10);

    printf(
        "REST server listening on %d\n",
        PORT);

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

        printf("%s\n", buffer);

        const char* response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: 30\r\n"
            "\r\n"
            "{\"status\":\"running\",\"cpu\":42}";

        write(
            client_fd,
            response,
            strlen(response));

        close(client_fd);
    }

    close(server_fd);

    return 0;
}



