#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/wait.h>
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

    int fd = create_memfd("shared_ipc");

    if (fd == -1)
    {
        perror("memfd_create");
        return 1;
    }

    if (ftruncate(fd, SIZE) == -1)
    {
        perror("ftruncate");
        return 1;
    }

    char* shared = mmap(
        NULL,
        SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );

    if (shared == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    strcpy(shared, "Message from parent");

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child received: %s\n", shared);

        strcpy(shared, "Message modified by child");

        munmap(shared, SIZE);
        close(fd);

        _exit(0);
    }

    waitpid(pid, NULL, 0);

    printf("Parent sees: %s\n", shared);

    munmap(shared, SIZE);
    close(fd);

    return 0;
}



