#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define SERVER_IP "127.0.0.1"

int main()
{
    int sock;

    struct sockaddr_in server_addr;

    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock,
        (struct sockaddr*)&server_addr,
        sizeof(server_addr)) < 0)
    {
        perror("connect");
        close(sock);
        return 1;
    }

    const char* message = "Hello distributed server";

    write(sock, message, strlen(message));

    memset(buffer, 0, sizeof(buffer));

    read(sock, buffer, sizeof(buffer) - 1);

    printf("Response: %s\n", buffer);

    close(sock);

    return 0;
}



