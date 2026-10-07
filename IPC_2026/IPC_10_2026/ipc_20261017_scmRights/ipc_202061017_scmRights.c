#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    int sv[2];

    socketpair(AF_UNIX,
        SOCK_DGRAM,
        0,
        sv);

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        close(sv[0]);

        char data;

        struct iovec io = {
            .iov_base = &data,
            .iov_len = 1
        };

        char control[CMSG_SPACE(sizeof(int))];

        struct msghdr msg = { 0 };

        msg.msg_iov = &io;
        msg.msg_iovlen = 1;

        msg.msg_control = control;
        msg.msg_controllen = sizeof(control);

        recvmsg(sv[1],
            &msg,
            0);

        struct cmsghdr* cmsg =
            CMSG_FIRSTHDR(&msg);

        if (cmsg &&
            cmsg->cmsg_level == SOL_SOCKET &&
            cmsg->cmsg_type == SCM_RIGHTS)
        {
            int received_fd;

            memcpy(&received_fd,
                CMSG_DATA(cmsg),
                sizeof(received_fd));

            printf("Received FD: %d\n",
                received_fd);

            write(received_fd,
                "Child wrote using passed FD\n",
                29);

            close(received_fd);
        }

        close(sv[1]);

        _exit(0);
    }

    close(sv[1]);

    int fd = open("scm_rights.txt",
        O_CREAT | O_RDWR | O_TRUNC,
        0666);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    char data = 'X';

    struct iovec io = {
        .iov_base = &data,
        .iov_len = 1
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

    printf("Sending FD: %d\n", fd);

    sendmsg(sv[0],
        &msg,
        0);

    close(fd);
    close(sv[0]);

    waitpid(pid, NULL, 0);

    return 0;
}



