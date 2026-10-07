#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/fd_server.sock"

static void send_fd(int socket_fd, int fd)
{
    char data = 'F';

    struct iovec io = {
        .iov_base = &data,
        .iov_len = sizeof(data)
    };

    char control[CMSG_SPACE(sizeof(int))];

    memset(control, 0, sizeof(control));

    struct msghdr msg = { 0 };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    msg.msg_control = control;
    msg.msg_controllen = sizeof(control);

    struct cmsghdr* cmsg =
        CMSG_FIRSTHDR(&msg);

    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));

    memcpy(CMSG_DATA(cmsg),
        &fd,
        sizeof(fd));

    msg.msg_controllen = cmsg->cmsg_len;

    if (sendmsg(socket_fd,
        &msg,
        0) == -1)
    {
        perror("sendmsg");
    }
}

int main(void)
{
    unlink(SOCKET_PATH);

    int server =
        socket(AF_UNIX, SOCK_STREAM, 0);

    if (server == -1)
    {
        perror("socket");
        return 1;
    }

    struct sockaddr_un addr = { 0 };

    addr.sun_family = AF_UNIX;

    strcpy(addr.sun_path,
        SOCKET_PATH);

    if (bind(server,
        (struct sockaddr*)&addr,
        sizeof(addr)) == -1)
    {
        perror("bind");
        return 1;
    }

    listen(server, 5);

    printf("FD server waiting...\n");

    int client =
        accept(server, NULL, NULL);

    if (client == -1)
    {
        perror("accept");
        return 1;
    }

    int fd = open("server_data.txt",
        O_CREAT | O_RDWR | O_TRUNC,
        0666);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    write(fd,
        "Data supplied by FD server\n",
        27);

    printf("Sending FD %d to client\n",
        fd);

    send_fd(client, fd);

    close(fd);
    close(client);
    close(server);

    unlink(SOCKET_PATH);

    return 0;
}



