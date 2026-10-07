#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/database_service.sock"

typedef struct
{
    int id;
    char name[32];
    int value;

} Record;

Record database[] =
{
    {1, "CPU", 45},
    {2, "MEMORY", 62},
    {3, "DISK", 70}
};

#define RECORD_COUNT 3

void query_database(int client_fd)
{
    char response[512];

    memset(response, 0, sizeof(response));

    for (int i = 0; i < RECORD_COUNT; i++)
    {
        char row[128];

        snprintf(row,
            sizeof(row),
            "ID=%d NAME=%s VALUE=%d\n",
            database[i].id,
            database[i].name,
            database[i].value);

        strcat(response, row);
    }

    write(client_fd,
        response,
        strlen(response) + 1);
}

int main(void)
{
    int server_fd;
    int client_fd;

    struct sockaddr_un address;

    char command[64];

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    unlink(SOCKET_PATH);

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    bind(server_fd,
        (struct sockaddr*)&address,
        sizeof(address));

    listen(server_fd, 5);

    printf("Database service started\n");

    while (1)
    {
        client_fd = accept(server_fd, NULL, NULL);

        if (client_fd < 0)
            continue;

        memset(command, 0, sizeof(command));

        read(client_fd,
            command,
            sizeof(command) - 1);

        if (strcmp(command, "SELECT") == 0)
        {
            query_database(client_fd);
        }
        else if (strcmp(command, "EXIT") == 0)
        {
            close(client_fd);
            break;
        }

        close(client_fd);
    }

    close(server_fd);

    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}



