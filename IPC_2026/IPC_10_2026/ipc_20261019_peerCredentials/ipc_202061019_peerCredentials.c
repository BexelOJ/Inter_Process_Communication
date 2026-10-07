#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(void)
{
    int sockets[2];

    if (socketpair(
        AF_UNIX,
        SOCK_STREAM,
        0,
        sockets) < 0)
    {
        perror("socketpair");
        return 1;
    }

    struct ucred credentials;
    socklen_t length = sizeof(credentials);

    if (getsockopt(
        sockets[0],
        SOL_SOCKET,
        SO_PEERCRED,
        &credentials,
        &length) < 0)
    {
        perror("getsockopt");
        return 1;
    }

    printf("Peer credentials\n");
    printf("-----------------------------\n");

    printf("PID : %d\n", credentials.pid);
    printf("UID : %d\n", credentials.uid);
    printf("GID : %d\n", credentials.gid);

    close(sockets[0]);
    close(sockets[1]);

    return 0;
}



