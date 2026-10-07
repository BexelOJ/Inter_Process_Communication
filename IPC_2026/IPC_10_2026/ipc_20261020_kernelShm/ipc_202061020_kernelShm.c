#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

#define SHM_SIZE 4096

int main(void)
{
    char* shared = mmap(
        NULL,
        SHM_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );

    if (shared == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        munmap(shared, SHM_SIZE);
        return 1;
    }

    if (pid == 0)
    {
        sleep(1);

        printf(
            "Child reads shared memory: %s\n",
            shared
        );

        munmap(shared, SHM_SIZE);

        return 0;
    }

    strcpy(
        shared,
        "Hello from parent shared memory"
    );

    printf("Parent wrote shared memory\n");

    waitpid(pid, NULL, 0);

    munmap(shared, SHM_SIZE);

    return 0;
}



