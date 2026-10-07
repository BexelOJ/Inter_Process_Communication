#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdatomic.h>
#include <string.h>

#define BUFFER_SIZE 4096

struct SharedBuffer
{
    atomic_int ready;

    size_t length;

    char data[BUFFER_SIZE];
};

int main(void)
{
    struct SharedBuffer* shared =
        mmap(
            NULL,
            sizeof(struct SharedBuffer),
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

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        while (
            atomic_load_explicit(
                &shared->ready,
                memory_order_acquire
            ) == 0
            )
        {
        }

        /*
         * Consumer reads directly from shared memory.
         *
         * No second buffer is required.
         */
        printf("Consumer received:\n");
        printf("%.*s",
            (int)shared->length,
            shared->data);

        _exit(0);
    }

    const char* message =
        "Zero-copy IPC message\n";

    size_t length = strlen(message);

    /*
     * Producer writes directly into shared memory.
     */
    memcpy(
        shared->data,
        message,
        length
    );

    shared->length = length;

    atomic_store_explicit(
        &shared->ready,
        1,
        memory_order_release
    );

    waitpid(pid, NULL, 0);

    munmap(
        shared,
        sizeof(struct SharedBuffer)
    );

    return 0;
}



