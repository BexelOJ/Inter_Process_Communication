#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 6000

int main()
{
    int sock;

    struct sockaddr_in server;

    int a = 10;
    int b = 20;

    int result;

    sock = socket(
        AF_INET,
        SOCK_STREAM,
        0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &server.sin_addr);

    connect(
        sock,
        (struct sockaddr*)&server,
        sizeof(server));

    write(
        sock,
        &a,
        sizeof(a));

    write(
        sock,
        &b,
        sizeof(b));

    read(
        sock,
        &result,
        sizeof(result));

    printf(
        "RPC result = %d\n",
        result);

    close(sock);

    return 0;
}



