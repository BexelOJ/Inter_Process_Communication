#include <stdio.h>
#include <stdatomic.h>

#define QUEUE_SIZE 1024

struct Queue
{
    int data[QUEUE_SIZE];

    atomic_size_t head;
    atomic_size_t tail;
};

void queue_init(struct Queue* q)
{
    atomic_init(&q->head, 0);
    atomic_init(&q->tail, 0);
}

int queue_push(struct Queue* q, int value)
{
    size_t head =
        atomic_load_explicit(
            &q->head,
            memory_order_relaxed
        );

    size_t tail =
        atomic_load_explicit(
            &q->tail,
            memory_order_acquire
        );

    if (head - tail >= QUEUE_SIZE)
        return 0;

    q->data[head % QUEUE_SIZE] = value;

    atomic_store_explicit(
        &q->head,
        head + 1,
        memory_order_release
    );

    return 1;
}

int queue_pop(struct Queue* q, int* value)
{
    size_t tail =
        atomic_load_explicit(
            &q->tail,
            memory_order_relaxed
        );

    size_t head =
        atomic_load_explicit(
            &q->head,
            memory_order_acquire
        );

    if (tail == head)
        return 0;

    *value = q->data[tail % QUEUE_SIZE];

    atomic_store_explicit(
        &q->tail,
        tail + 1,
        memory_order_release
    );

    return 1;
}

int main(void)
{
    struct Queue queue;

    queue_init(&queue);

    queue_push(&queue, 100);
    queue_push(&queue, 200);

    int value;

    while (queue_pop(&queue, &value))
    {
        printf("Received: %d\n", value);
    }

    return 0;
}



