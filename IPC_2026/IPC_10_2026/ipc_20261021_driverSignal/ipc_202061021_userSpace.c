#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int fd;

void signal_handler(int signal)
{
    printf(
        "SIGIO received from kernel driver\n"
    );
}

int main(void)
{
    fd = open(
        "/dev/ipc_driver_signal",
        O_RDWR | O_NONBLOCK
    );

    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    signal(SIGIO, signal_handler);

    fcntl(
        fd,
        F_SETOWN,
        getpid()
    );

    int flags = fcntl(
        fd,
        F_GETFL
    );

    fcntl(
        fd,
        F_SETFL,
        flags | FASYNC
    );

    printf(
        "Waiting for SIGIO from driver...\n"
    );

    while (1)
        pause();

    close(fd);

    return 0;
}




