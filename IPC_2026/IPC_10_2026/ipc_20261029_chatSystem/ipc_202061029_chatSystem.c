// ---------------------------------------------------
// ipc_20261029_chatSystem
// TCP multi-client chat server using select()
// ---------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 5000
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main(void)
{
    int server_fd;
    int clients[MAX_CLIENTS];
    int max_fd;
    fd_set readfds;

    char buffer[BUFFER_SIZE];

    struct sockaddr_in server_addr;

    for (int i = 0; i < MAX_CLIENTS; i++)
        clients[i] = -1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("socket");
        return 1;
    }

    int opt = 1;

    setsockopt(server_fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));

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

    if (listen(server_fd, 10) < 0)
    {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Chat server listening on port %d...\n", PORT);

    while (1)
    {
        FD_ZERO(&readfds);

        FD_SET(server_fd, &readfds);
        max_fd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i] != -1)
            {
                FD_SET(clients[i], &readfds);

                if (clients[i] > max_fd)
                    max_fd = clients[i];
            }
        }

        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0)
        {
            perror("select");
            break;
        }

        /* New client */
        if (FD_ISSET(server_fd, &readfds))
        {
            int client_fd = accept(server_fd, NULL, NULL);

            if (client_fd >= 0)
            {
                for (int i = 0; i < MAX_CLIENTS; i++)
                {
                    if (clients[i] == -1)
                    {
                        clients[i] = client_fd;
                        printf("Client connected: fd=%d\n", client_fd);
                        break;
                    }
                }
            }
        }

        /* Existing clients */
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            int fd = clients[i];

            if (fd == -1)
                continue;

            if (FD_ISSET(fd, &readfds))
            {
                int n = read(fd, buffer, sizeof(buffer) - 1);

                if (n <= 0)
                {
                    printf("Client disconnected: fd=%d\n", fd);
                    close(fd);
                    clients[i] = -1;
                    continue;
                }

                buffer[n] = '\0';

                printf("Client %d: %s", fd, buffer);

                /* Broadcast */
                for (int j = 0; j < MAX_CLIENTS; j++)
                {
                    if (clients[j] != -1 && clients[j] != fd)
                    {
                        write(clients[j], buffer, n);
                    }
                }
            }
        }
    }

    close(server_fd);

    return 0;
}



