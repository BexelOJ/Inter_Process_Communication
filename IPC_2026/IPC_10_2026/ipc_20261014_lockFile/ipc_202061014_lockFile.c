#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
    const char* lockFile = "/tmp/my_application.lock";

    int fd = open(lockFile,
        O_CREAT | O_EXCL | O_WRONLY,
        0644);

    if (fd == -1)
    {
        if (errno == EEXIST)
        {
            printf("Application is already running\n");
            return 1;
        }

        perror("open");
        return 1;
    }

    dprintf(fd, "%d\n", getpid());

    printf("Application started\n");
    printf("PID: %d\n", getpid());

    printf("Running...\n");

    while (1)
    {
        sleep(1);
    }

    close(fd);
    unlink(lockFile);

    return 0;
}



