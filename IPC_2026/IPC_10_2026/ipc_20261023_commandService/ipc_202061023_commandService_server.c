#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/command_service.sock"

int main(void)
{
    int server_fd;
    int client_fd;

    struct sockaddr_un address;

    char command[128];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    unlink(SOCKET_PATH);

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    if (bind(server_fd,
        (struct sockaddr*)&address,
        sizeof(address)) < 0)
    {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Command service started\n");

    while (1)
    {
        client_fd = accept(server_fd, NULL, NULL);

        if (client_fd < 0)
        {
            perror("accept");
            continue;
        }

        memset(command, 0, sizeof(command));

        read(client_fd,
            command,
            sizeof(command) - 1);

        printf("Command received: %s\n", command);

        if (strcmp(command, "START") == 0)
        {
            printf("Executing START\n");
        }
        else if (strcmp(command, "STOP") == 0)
        {
            printf("Executing STOP\n");
        }
        else if (strcmp(command, "STATUS") == 0)
        {
            printf("Executing STATUS\n");
        }
        else if (strcmp(command, "EXIT") == 0)
        {
            printf("Stopping command service\n");

            close(client_fd);
            break;
        }
        else
        {
            printf("Unknown command\n");
        }

        close(client_fd);
    }

    close(server_fd);

    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}



