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

    int a;
    int b;
    int result;

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

    listen(server_fd, 5);

    printf("RPC server running...\n");

    client_fd = accept(
        server_fd,
        NULL,
        NULL);

    read(
        client_fd,
        &a,
        sizeof(a));

    read(
        client_fd,
        &b,
        sizeof(b));

    result = a + b;

    write(
        client_fd,
        &result,
        sizeof(result));

    printf(
        "RPC: %d + %d = %d\n",
        a,
        b,
        result);

    close(client_fd);
    close(server_fd);

    return 0;
}



