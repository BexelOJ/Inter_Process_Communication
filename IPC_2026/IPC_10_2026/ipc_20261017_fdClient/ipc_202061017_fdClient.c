#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/fd_server.sock"

int main(void)
{
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock == -1)
    {
        perror("socket");
        return 1;
    }

    struct sockaddr_un addr = { 0 };

    addr.sun_family = AF_UNIX;

    snprintf(addr.sun_path,
        sizeof(addr.sun_path),
        "%s",
        SOCKET_PATH);

    if (connect(sock,
        (struct sockaddr*)&addr,
        sizeof(addr)) == -1)
    {
        perror("connect");
        close(sock);
        return 1;
    }

    printf("Connected to FD server\n");

    char buffer[64];

    ssize_t n = recv(sock,
        buffer,
        sizeof(buffer) - 1,
        0);

    if (n > 0)
    {
        buffer[n] = '\0';

        printf("Server says: %s\n",
            buffer);
    }

    close(sock);

    return 0;
}



