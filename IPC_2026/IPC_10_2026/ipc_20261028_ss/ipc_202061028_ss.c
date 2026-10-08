#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 6000

int main()
{
    int server_fd;
    int client_fd;

    struct sockaddr_in address;

    server_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0);

    int option = 1;

    setsockopt(
        server_fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &option,
        sizeof(option));

    address.sin_family = AF_INET;

    address.sin_addr.s_addr =
        INADDR_ANY;

    address.sin_port =
        htons(PORT);

    if (bind(
        server_fd,
        (struct sockaddr*)&address,
        sizeof(address)) < 0)
    {
        perror("bind");
        return 1;
    }

    listen(server_fd, 5);

    printf(
        "Server listening on %d\n",
        PORT);

    printf(
        "Run:\n");

    printf(
        "ss -lntp | grep %d\n",
        PORT);

    client_fd = accept(
        server_fd,
        NULL,
        NULL);

    if (client_fd >= 0)
    {
        printf(
            "Client connected\n");

        char buffer[256];

        int bytes = read(
            client_fd,
            buffer,
            sizeof(buffer) - 1);

        if (bytes > 0)
        {
            buffer[bytes] = '\0';

            printf(
                "Received: %s\n",
                buffer);
        }

        close(client_fd);
    }

    close(server_fd);

    return 0;
}



