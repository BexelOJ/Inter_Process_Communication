#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void)
{
    int fd = open("shared_fd.txt",
        O_CREAT | O_RDWR | O_TRUNC,
        0666);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    printf("Parent FD: %d\n", fd);

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child FD: %d\n", fd);

        write(fd,
            "Child wrote this\n",
            17);

        close(fd);

        _exit(0);
    }

    write(fd,
        "Parent wrote this\n",
        18);

    waitpid(pid, NULL, 0);

    close(fd);

    printf("Both processes used the same open file\n");

    return 0;
}



