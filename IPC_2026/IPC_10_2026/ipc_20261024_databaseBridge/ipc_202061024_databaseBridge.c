#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <sqlite3.h>

#define SOCKET_PATH "/tmp/database_bridge.sock"
#define DATABASE    "sensor.db"

typedef struct
{
    int sensor_id;
    float temperature;
    float humidity;

} SensorData;

static sqlite3* db;

static int database_init(void)
{
    int rc;

    rc = sqlite3_open(DATABASE, &db);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr,
            "Database error: %s\n",
            sqlite3_errmsg(db));

        return -1;
    }

    const char* sql =
        "CREATE TABLE IF NOT EXISTS sensor_data ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "sensor_id INTEGER,"
        "temperature REAL,"
        "humidity REAL,"
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");";

    char* error = NULL;

    rc = sqlite3_exec(db,
        sql,
        NULL,
        NULL,
        &error);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr,
            "SQL error: %s\n",
            error);

        sqlite3_free(error);

        return -1;
    }

    return 0;
}

static int database_insert(const SensorData* data)
{
    char sql[256];

    snprintf(sql,
        sizeof(sql),
        "INSERT INTO sensor_data "
        "(sensor_id, temperature, humidity) "
        "VALUES (%d, %.2f, %.2f);",
        data->sensor_id,
        data->temperature,
        data->humidity);

    return sqlite3_exec(db,
        sql,
        NULL,
        NULL,
        NULL);
}

int main(void)
{
    int server_fd;
    int client_fd;

    struct sockaddr_un address;

    if (database_init() < 0)
        return EXIT_FAILURE;

    server_fd = socket(AF_UNIX,
        SOCK_STREAM,
        0);

    if (server_fd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    unlink(SOCKET_PATH);

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;

    strcpy(address.sun_path,
        SOCKET_PATH);

    if (bind(server_fd,
        (struct sockaddr*)&address,
        sizeof(address)) < 0)
    {
        perror("bind");

        close(server_fd);

        return EXIT_FAILURE;
    }

    listen(server_fd, 5);

    printf("Database bridge started\n");

    while (1)
    {
        client_fd = accept(server_fd,
            NULL,
            NULL);

        if (client_fd < 0)
            continue;

        SensorData data;

        ssize_t bytes =
            read(client_fd,
                &data,
                sizeof(data));

        if (bytes == sizeof(data))
        {
            printf("Received sensor %d\n",
                data.sensor_id);

            database_insert(&data);
        }

        close(client_fd);
    }

    sqlite3_close(db);

    close(server_fd);

    unlink(SOCKET_PATH);

    return EXIT_SUCCESS;
}



