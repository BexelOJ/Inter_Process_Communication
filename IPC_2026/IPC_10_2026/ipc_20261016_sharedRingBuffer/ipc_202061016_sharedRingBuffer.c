#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdatomic.h>

#define RING_SIZE 1024
#define ITEMS 100000

struct RingBuffer
{
    int data[RING_SIZE];

    atomic_size_t head;
    atomic_size_t tail;
};

int push(struct RingBuffer* rb, int value)
{
    size_t head =
        atomic_load_explicit(
            &rb->head,
            memory_order_relaxed
        );

    size_t tail =
        atomic_load_explicit(
            &rb->tail,
            memory_order_acquire
        );

    if (head - tail >= RING_SIZE)
        return 0;

    rb->data[head % RING_SIZE] = value;

    atomic_store_explicit(
        &rb->head,
        head + 1,
        memory_order_release
    );

    return 1;
}

int pop(struct RingBuffer* rb, int* value)
{
    size_t tail =
        atomic_load_explicit(
            &rb->tail,
            memory_order_relaxed
        );

    size_t head =
        atomic_load_explicit(
            &rb->head,
            memory_order_acquire
        );

    if (tail == head)
        return 0;

    *value = rb->data[tail % RING_SIZE];

    atomic_store_explicit(
        &rb->tail,
        tail + 1,
        memory_order_release
    );

    return 1;
}

int main(void)
{
    struct RingBuffer* rb = mmap(
        NULL,
        sizeof(struct RingBuffer),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );

    if (rb == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    atomic_init(&rb->head, 0);
    atomic_init(&rb->tail, 0);

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        int value;

        for (int i = 0; i < ITEMS; i++)
        {
            while (!pop(rb, &value))
            {
            }
        }

        printf("Consumer received %d items\n",
            ITEMS);

        _exit(0);
    }

    for (int i = 0; i < ITEMS; i++)
    {
        while (!push(rb, i))
        {
        }
    }

    waitpid(pid, NULL, 0);

    printf("Producer sent %d items\n", ITEMS);

    munmap(rb, sizeof(struct RingBuffer));

    return 0;
}



