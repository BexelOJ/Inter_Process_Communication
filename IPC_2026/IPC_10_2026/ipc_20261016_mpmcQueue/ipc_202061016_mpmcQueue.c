#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define QUEUE_SIZE 64
#define PRODUCERS 2
#define CONSUMERS 2
#define ITEMS_PER_PRODUCER 10000

struct Queue
{
    int data[QUEUE_SIZE];

    int head;
    int tail;
    int count;

    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
};

struct Queue queue;

void queue_init(void)
{
    queue.head = 0;
    queue.tail = 0;
    queue.count = 0;

    pthread_mutex_init(&queue.mutex, NULL);
    pthread_cond_init(&queue.not_empty, NULL);
    pthread_cond_init(&queue.not_full, NULL);
}

void queue_push(int value)
{
    pthread_mutex_lock(&queue.mutex);

    while (queue.count == QUEUE_SIZE)
        pthread_cond_wait(
            &queue.not_full,
            &queue.mutex
        );

    queue.data[queue.head] = value;

    queue.head =
        (queue.head + 1) % QUEUE_SIZE;

    queue.count++;

    pthread_cond_signal(&queue.not_empty);

    pthread_mutex_unlock(&queue.mutex);
}

int queue_pop(int* value)
{
    pthread_mutex_lock(&queue.mutex);

    while (queue.count == 0)
        pthread_cond_wait(
            &queue.not_empty,
            &queue.mutex
        );

    *value = queue.data[queue.tail];

    queue.tail =
        (queue.tail + 1) % QUEUE_SIZE;

    queue.count--;

    pthread_cond_signal(&queue.not_full);

    pthread_mutex_unlock(&queue.mutex);

    return 1;
}

void* producer(void* arg)
{
    int id = *(int*)arg;

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++)
    {
        queue_push(id * ITEMS_PER_PRODUCER + i);
    }

    return NULL;
}

void* consumer(void* arg)
{
    (void)arg;

    int value;

    for (;;)
    {
        if (queue_pop(&value))
        {
            printf("Consumer received %d\n", value);
        }
    }

    return NULL;
}

int main(void)
{
    queue_init();

    pthread_t producers[PRODUCERS];

    int producer_id[PRODUCERS];

    for (int i = 0; i < PRODUCERS; i++)
    {
        producer_id[i] = i;

        pthread_create(
            &producers[i],
            NULL,
            producer,
            &producer_id[i]
        );
    }

    /*
     * For a complete production implementation,
     * consumers need a termination condition.
     */

    for (int i = 0; i < PRODUCERS; i++)
        pthread_join(producers[i], NULL);

    printf("All producers completed\n");

    return 0;
}



