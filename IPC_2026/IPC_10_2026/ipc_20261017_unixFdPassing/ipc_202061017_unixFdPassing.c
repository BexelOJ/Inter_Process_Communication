#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>

static int send_fd(int socket_fd, int fd)
{
    char data = 'F';

    struct iovec io;

    io.iov_base = &data;
    io.iov_len = sizeof(data);

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

    return sendmsg(socket_fd,
        &msg,
        0);
}

static int receive_fd(int socket_fd)
{
    char data;

    struct iovec io;

    io.iov_base = &data;
    io.iov_len = sizeof(data);

    char control[CMSG_SPACE(sizeof(int))];

    memset(control, 0, sizeof(control));

    struct msghdr msg = { 0 };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    msg.msg_control = control;
    msg.msg_controllen = sizeof(control);

    if (recvmsg(socket_fd,
        &msg,
        0) == -1)
    {
        perror("recvmsg");
        return -1;
    }

    struct cmsghdr* cmsg =
        CMSG_FIRSTHDR(&msg);

    if (cmsg == NULL)
        return -1;

    if (cmsg->cmsg_level != SOL_SOCKET ||
        cmsg->cmsg_type != SCM_RIGHTS)
    {
        return -1;
    }

    int fd;

    memcpy(&fd,
        CMSG_DATA(cmsg),
        sizeof(fd));

    return fd;
}

int main(void)
{
    int sv[2];

    if (socketpair(AF_UNIX,
        SOCK_DGRAM,
        0,
        sv) == -1)
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
        close(sv[0]);

        printf("Client waiting for FD...\n");

        int fd = receive_fd(sv[1]);

        if (fd == -1)
        {
            fprintf(stderr,
                "Failed to receive FD\n");
            _exit(1);
        }

        printf("Client received FD: %d\n",
            fd);

        lseek(fd, 0, SEEK_SET);

        char buffer[256] = { 0 };

        ssize_t n = read(fd,
            buffer,
            sizeof(buffer) - 1);

        if (n > 0)
        {
            printf("Client read:\n%s",
                buffer);
        }

        close(fd);
        close(sv[1]);

        _exit(0);
    }

    close(sv[1]);

    int fd = open("unix_passed_file.txt",
        O_CREAT | O_RDWR | O_TRUNC,
        0666);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    write(fd,
        "This file descriptor came through a Unix socket.\n",
        49);

    printf("Server opened FD: %d\n",
        fd);

    printf("Server passing FD...\n");

    if (send_fd(sv[0], fd) == -1)
    {
        perror("send_fd");
    }

    close(fd);
    close(sv[0]);

    waitpid(pid, NULL, 0);

    return 0;
}



