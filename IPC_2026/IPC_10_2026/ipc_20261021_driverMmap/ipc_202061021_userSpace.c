#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define MAP_SIZE 4096

int main(void)
{
    int fd = open(
        "/dev/ipc_driver_mmap",
        O_RDWR
    );

    char *buffer = mmap(
        NULL,
        MAP_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );

    if (buffer == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    printf(
        "Userspace mapped data:\n%s",
        buffer
    );

    munmap(buffer, MAP_SIZE);
    close(fd);

    return 0;
}




