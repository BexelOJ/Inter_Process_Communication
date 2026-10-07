#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 6000

int main()
{
    int sock;

    struct sockaddr_in server;

    char message[] =
        "temperature=27.5";

    char response[1024];

    sock = socket(
        AF_INET,
        SOCK_STREAM,
        0);

    server.sin_family = AF_INET;

    server.sin_port =
        htons(PORT);

    inet_pton(
        AF_INET,
        SERVER_IP,
        &server.sin_addr);

    connect(
        sock,
        (struct sockaddr*)&server,
        sizeof(server));

    write(
        sock,
        message,
        strlen(message));

    memset(
        response,
        0,
        sizeof(response));

    read(
        sock,
        response,
        sizeof(response) - 1);

    printf(
        "Service A response: %s\n",
        response);

    close(sock);

    return 0;
}



