#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main()
{
    int server_fd;
    int client_fd;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    socklen_t client_len = sizeof(client_addr);

    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd,
        (struct sockaddr*)&server_addr,
        sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(server_fd);
        return 1;
    }

    listen(server_fd, 5);

    printf("Distributed server running on port %d...\n", PORT);

    client_fd = accept(server_fd,
        (struct sockaddr*)&client_addr,
        &client_len);

    if (client_fd < 0)
    {
        perror("accept");
        close(server_fd);
        return 1;
    }

    memset(buffer, 0, sizeof(buffer));

    read(client_fd, buffer, sizeof(buffer) - 1);

    printf("Request: %s\n", buffer);

    const char* response = "Hello from distributed server";

    write(client_fd, response, strlen(response));

    close(client_fd);
    close(server_fd);

    return 0;
}



