#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/request_response.sock"

int main(void)
{
    int fd;

    struct sockaddr_un address;

    char request[] = "GET_STATUS";
    char response[128];

    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    connect(fd,
        (struct sockaddr*)&address,
        sizeof(address));

    write(fd,
        request,
        strlen(request) + 1);

    read(fd,
        response,
        sizeof(response));

    printf("Server response: %s\n", response);

    close(fd);

    return EXIT_SUCCESS;
}



