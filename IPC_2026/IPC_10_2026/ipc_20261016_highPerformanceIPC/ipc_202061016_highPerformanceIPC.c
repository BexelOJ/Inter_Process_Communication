#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdatomic.h>

#define ITERATIONS 1000000

struct SharedData
{
    atomic_int ready;
    atomic_int value;
};

int main(void)
{
    struct SharedData* shared = mmap(
        NULL,
        sizeof(struct SharedData),
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

    atomic_init(&shared->ready, 0);
    atomic_init(&shared->value, 0);

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        for (int i = 0; i < ITERATIONS; i++)
        {
            while (atomic_load_explicit(
                &shared->ready,
                memory_order_acquire) == 0)
            {
            }

            atomic_store_explicit(
                &shared->ready,
                0,
                memory_order_release
            );
        }

        _exit(0);
    }

    for (int i = 0; i < ITERATIONS; i++)
    {
        atomic_store_explicit(
            &shared->value,
            i,
            memory_order_relaxed
        );

        atomic_store_explicit(
            &shared->ready,
            1,
            memory_order_release
        );

        while (atomic_load_explicit(
            &shared->ready,
            memory_order_acquire) != 0)
        {
        }
    }

    waitpid(pid, NULL, 0);

    printf("High-performance IPC completed\n");
    printf("Iterations: %d\n", ITERATIONS);

    munmap(shared, sizeof(struct SharedData));

    return 0;
}



