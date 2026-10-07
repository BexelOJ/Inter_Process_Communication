#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/command_service.sock"

int main(int argc, char *argv[])
{
    int fd;

    struct sockaddr_un address;

    if (argc != 2)
    {
        printf("Usage: %s START|STOP|STATUS|EXIT\n", argv[0]);
        return EXIT_FAILURE;
    }

    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (fd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    if (connect(fd,
                (struct sockaddr *)&address,
                sizeof(address)) < 0)
    {
        perror("connect");
        close(fd);
        return EXIT_FAILURE;
    }

    write(fd,
          argv[1],
          strlen(argv[1]) + 1);

    close(fd);

    return EXIT_SUCCESS;
}




