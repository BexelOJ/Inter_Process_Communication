// ---------------------------------------------------
// ipc_20261029_databaseServer
// Unix socket + SQLite database server
// ---------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sqlite3.h>

#define SOCKET_PATH "/tmp/database_server.sock"

int main(void)
{
    int server_fd;
    int client_fd;

    struct sockaddr_un addr;

    sqlite3* db;

    unlink(SOCKET_PATH);

    if (sqlite3_open("ipc_database.db", &db) != SQLITE_OK)
    {
        fprintf(stderr, "Database error\n");
        return 1;
    }

    sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS data("
        "id INTEGER PRIMARY KEY,"
        "value TEXT);",
        NULL,
        NULL,
        NULL);

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;

    strncpy(addr.sun_path,
        SOCKET_PATH,
        sizeof(addr.sun_path) - 1);

    bind(server_fd,
        (struct sockaddr*)&addr,
        sizeof(addr));

    listen(server_fd, 5);

    printf("Database server waiting...\n");

    while (1)
    {
        client_fd = accept(server_fd, NULL, NULL);

        char buffer[256];

        int n = read(client_fd,
            buffer,
            sizeof(buffer) - 1);

        if (n > 0)
        {
            buffer[n] = '\0';

            char sql[512];

            snprintf(sql,
                sizeof(sql),
                "INSERT INTO data(value) VALUES('%s');",
                buffer);

            sqlite3_exec(db,
                sql,
                NULL,
                NULL,
                NULL);

            write(client_fd,
                "Database updated\n",
                18);
        }

        close(client_fd);
    }

    sqlite3_close(db);
    close(server_fd);

    unlink(SOCKET_PATH);

    return 0;
}



