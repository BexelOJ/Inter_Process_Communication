#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/container_ipc_shm"
#define SHM_SIZE 4096

int main()
{
    int fd = shm_open(
        SHM_NAME,
        O_CREAT | O_RDWR,
        0666);

    if (fd < 0)
    {
        perror("shm_open");
        return 1;
    }

    ftruncate(fd, SHM_SIZE);

    char* memory = mmap(
        NULL,
        SHM_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0);

    if (memory == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    strcpy(
        memory,
        "Hello from container IPC");

    printf("Message written\n");

    munmap(memory, SHM_SIZE);

    close(fd);

    return 0;
}



