#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>

void send_fd(int socket_fd, int fd)
{
    struct msghdr msg = { 0 };

    char buffer[1] = { 'F' };

    struct iovec io;

    io.iov_base = buffer;
    io.iov_len = sizeof(buffer);

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    char control[CMSG_SPACE(sizeof(int))];

    memset(control, 0, sizeof(control));

    msg.msg_control = control;
    msg.msg_controllen = sizeof(control);

    struct cmsghdr* cmsg =
        CMSG_FIRSTHDR(&msg);

    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));

    memcpy(CMSG_DATA(cmsg),
        &fd,
        sizeof(int));

    msg.msg_controllen = cmsg->cmsg_len;

    if (sendmsg(socket_fd, &msg, 0) == -1)
    {
        perror("sendmsg");
        exit(1);
    }
}

int receive_fd(int socket_fd)
{
    struct msghdr msg = { 0 };

    char buffer[1];

    struct iovec io;

    io.iov_base = buffer;
    io.iov_len = sizeof(buffer);

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    char control[CMSG_SPACE(sizeof(int))];

    memset(control, 0, sizeof(control));

    msg.msg_control = control;
    msg.msg_controllen = sizeof(control);

    if (recvmsg(socket_fd, &msg, 0) == -1)
    {
        perror("recvmsg");
        exit(1);
    }

    struct cmsghdr* cmsg =
        CMSG_FIRSTHDR(&msg);

    if (cmsg == NULL)
    {
        fprintf(stderr,
            "No FD received\n");
        exit(1);
    }

    int fd;

    memcpy(&fd,
        CMSG_DATA(cmsg),
        sizeof(int));

    return fd;
}

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

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        close(sockets[0]);

        int fd = receive_fd(sockets[1]);

        printf("Child received FD: %d\n",
            fd);

        lseek(fd, 0, SEEK_SET);

        char buffer[128] = { 0 };

        read(fd,
            buffer,
            sizeof(buffer) - 1);

        printf("Child read: %s",
            buffer);

        close(fd);
        close(sockets[1]);

        _exit(0);
    }

    close(sockets[1]);

    int fd = open("shared.txt",
        O_CREAT | O_RDWR | O_TRUNC,
        0666);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    write(fd,
        "Hello through passed FD\n",
        24);

    printf("Parent sending FD: %d\n",
        fd);

    send_fd(sockets[0], fd);

    close(fd);
    close(sockets[0]);

    waitpid(pid, NULL, 0);

    return 0;
}



