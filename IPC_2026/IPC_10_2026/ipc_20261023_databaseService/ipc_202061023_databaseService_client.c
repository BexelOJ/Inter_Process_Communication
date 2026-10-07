#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/database_service.sock"

int main(void)
{
    int fd;

    struct sockaddr_un address;

    char response[512];

    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    connect(fd,
            (struct sockaddr *)&address,
            sizeof(address));

    write(fd,
          "SELECT",
          strlen("SELECT") + 1);

    memset(response, 0, sizeof(response));

    read(fd,
         response,
         sizeof(response) - 1);

    printf("Database response:\n%s",
           response);

    close(fd);

    return EXIT_SUCCESS;
}




