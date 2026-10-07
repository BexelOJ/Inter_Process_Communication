#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
    const char* lockFile = "application.lock";

    int fd = open(lockFile, O_CREAT | O_EXCL | O_WRONLY, 0666);

    if (fd == -1)
    {
        if (errno == EEXIST)
        {
            printf("Another process already owns the lock\n");
        }
        else
        {
            perror("open");
        }

        return 1;
    }

    printf("Lock acquired\n");

    dprintf(fd, "%d\n", getpid());

    printf("Critical section...\n");

    sleep(10);

    close(fd);

    unlink(lockFile);

    printf("Lock released\n");

    return 0;
}



