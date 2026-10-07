#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

int main(void)
{
    int sv[2];

    if (socketpair(AF_UNIX,
        SOCK_STREAM,
        0,
        sv) == -1)
    {
        perror("socketpair");
        return 1;
    }

    char part1[] = "Hello ";
    char part2[] = "from ";
    char part3[] = "sendmsg()";

    struct iovec iov[3];

    iov[0].iov_base = part1;
    iov[0].iov_len = strlen(part1);

    iov[1].iov_base = part2;
    iov[1].iov_len = strlen(part2);

    iov[2].iov_base = part3;
    iov[2].iov_len = strlen(part3);

    struct msghdr msg = { 0 };

    msg.msg_iov = iov;
    msg.msg_iovlen = 3;

    if (sendmsg(sv[0],
        &msg,
        0) == -1)
    {
        perror("sendmsg");
        return 1;
    }

    char buffer[128] = { 0 };

    read(sv[1],
        buffer,
        sizeof(buffer) - 1);

    printf("Received: %s\n",
        buffer);

    close(sv[0]);
    close(sv[1]);

    return 0;
}



