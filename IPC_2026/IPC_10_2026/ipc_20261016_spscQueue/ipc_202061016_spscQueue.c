#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define QUEUE_SIZE 1024
#define ITEMS 1000000

struct Queue
{
    int data[QUEUE_SIZE];

    atomic_size_t head;
    atomic_size_t tail;
};

struct Queue queue;

void* producer(void* arg)
{
    (void)arg;

    for (int i = 0; i < ITEMS; i++)
    {
        size_t head =
            atomic_load_explicit(
                &queue.head,
                memory_order_relaxed
            );

        while (
            head -
            atomic_load_explicit(
                &queue.tail,
                memory_order_acquire
            )
            >= QUEUE_SIZE
            )
        {
            sched_yield();
        }

        queue.data[head % QUEUE_SIZE] = i;

        atomic_store_explicit(
            &queue.head,
            head + 1,
            memory_order_release
        );
    }

    return NULL;
}

void* consumer(void* arg)
{
    (void)arg;

    int received = 0;

    while (received < ITEMS)
    {
        size_t tail =
            atomic_load_explicit(
                &queue.tail,
                memory_order_relaxed
            );

        size_t head =
            atomic_load_explicit(
                &queue.head,
                memory_order_acquire
            );

        if (tail == head)
        {
            sched_yield();
            continue;
        }

        int value =
            queue.data[tail % QUEUE_SIZE];

        if (received < 10)
            printf("Received: %d\n", value);

        received++;

        atomic_store_explicit(
            &queue.tail,
            tail + 1,
            memory_order_release
        );
    }

    return NULL;
}

int main(void)
{
    atomic_init(&queue.head, 0);
    atomic_init(&queue.tail, 0);

    pthread_t producer_thread;
    pthread_t consumer_thread;

    pthread_create(
        &producer_thread,
        NULL,
        producer,
        NULL
    );

    pthread_create(
        &consumer_thread,
        NULL,
        consumer,
        NULL
    );

    pthread_join(
        producer_thread,
        NULL
    );

    pthread_join(
        consumer_thread,
        NULL
    );

    printf("SPSC completed: %d items\n",
        ITEMS);

    return 0;
}



