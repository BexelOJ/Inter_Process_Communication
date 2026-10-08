#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000

int main()
{
    int server_fd;

    struct sockaddr_in address;

    server_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0);

    if (server_fd < 0)
    {
        perror("socket");
        return 1;
    }

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
        close(server_fd);
        return 1;
    }

    listen(server_fd, 5);

    printf(
        "TCP server listening on port %d\n",
        PORT);

    printf(
        "Try:\n");

    printf(
        "netstat -lntp | grep %d\n",
        PORT);

    sleep(60);

    close(server_fd);

    return 0;
}



