#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

int main(void)
{
    int sockets[2];

    if (socketpair(AF_UNIX,
        SOCK_STREAM,
        0,
        sockets) == -1)
    {
        perror("socketpair");
        return 1;
    }

    const char* message =
        "Hello using sendmsg";

    struct iovec io = {
        .iov_base = (void*)message,
        .iov_len = strlen(message) + 1
    };

    struct msghdr send_msg = { 0 };

    send_msg.msg_iov = &io;
    send_msg.msg_iovlen = 1;

    sendmsg(sockets[0],
        &send_msg,
        0);

    char buffer[128];

    struct iovec recv_io = {
        .iov_base = buffer,
        .iov_len = sizeof(buffer)
    };

    struct msghdr recv_msg = { 0 };

    recv_msg.msg_iov = &recv_io;
    recv_msg.msg_iovlen = 1;

    if (recvmsg(sockets[1],
        &recv_msg,
        0) == -1)
    {
        perror("recvmsg");
        return 1;
    }

    printf("Received: %s\n",
        buffer);

    close(sockets[0]);
    close(sockets[1]);

    return 0;
}



