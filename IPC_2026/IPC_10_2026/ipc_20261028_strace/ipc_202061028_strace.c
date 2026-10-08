#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;

    printf(
        "PID: %d\n",
        getpid());

    fd = open(
        "strace_test.txt",
        O_CREAT | O_WRONLY | O_TRUNC,
        0644);

    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    const char* message =
        "Hello from strace\n";

    write(
        fd,
        message,
        18);

    close(fd);

    return 0;
}



