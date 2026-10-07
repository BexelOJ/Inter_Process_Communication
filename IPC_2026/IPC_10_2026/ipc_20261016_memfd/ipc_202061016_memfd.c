#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <string.h>

#ifndef MFD_CLOEXEC
#define MFD_CLOEXEC 0x0001U
#endif

static int create_memfd(const char* name)
{
    return syscall(
        SYS_memfd_create,
        name,
        MFD_CLOEXEC
    );
}

int main(void)
{
    const size_t SIZE = 4096;

    int fd = create_memfd("ipc_memfd");

    if (fd == -1)
    {
        perror("memfd_create");
        return 1;
    }

    if (ftruncate(fd, SIZE) == -1)
    {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    char* memory = mmap(
        NULL,
        SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );

    if (memory == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    strcpy(memory, "Hello from memfd");

    printf("memfd says: %s\n", memory);
    printf("File descriptor: %d\n", fd);

    munmap(memory, SIZE);
    close(fd);

    return 0;
}



