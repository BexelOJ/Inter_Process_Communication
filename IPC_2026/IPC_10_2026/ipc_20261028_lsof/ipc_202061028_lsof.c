#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

int main()
{
    int file_fd;
    int socket_fd;

    file_fd = open(
        "test_file.txt",
        O_CREAT | O_RDWR,
        0666);

    if (file_fd < 0)
    {
        perror("open");
        return 1;
    }

    socket_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0);

    if (socket_fd < 0)
    {
        perror("socket");
        return 1;
    }

    printf(
        "PID: %d\n",
        getpid());

    printf(
        "File FD: %d\n",
        file_fd);

    printf(
        "Socket FD: %d\n",
        socket_fd);

    printf(
        "\nRun:\n");

    printf(
        "lsof -p %d\n",
        getpid());

    printf(
        "\nSleeping for 60 seconds...\n");

    sleep(60);

    close(file_fd);
    close(socket_fd);

    return 0;
}



