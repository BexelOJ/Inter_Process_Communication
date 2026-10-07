#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>

int main(void)
{
    int fd = open("flock.lock", O_CREAT | O_RDWR, 0666);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    printf("Waiting for flock...\n");

    if (flock(fd, LOCK_EX) == -1)
    {
        perror("flock");
        close(fd);
        return 1;
    }

    printf("Exclusive lock acquired\n");

    printf("Critical section\n");

    sleep(10);

    flock(fd, LOCK_UN);

    printf("Lock released\n");

    close(fd);

    return 0;
}



