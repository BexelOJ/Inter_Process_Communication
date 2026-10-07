#include <stdio.h>
#include <pthread.h>

#define QUEUE_SIZE 128
#define CONSUMERS 4
#define ITEMS 10000

struct Queue
{
    int data[QUEUE_SIZE];

    int head;
    int tail;
    int count;

    pthread_mutex_t mutex;
};

struct Queue queue;

void push(int value)
{
    pthread_mutex_lock(&queue.mutex);

    while (queue.count == QUEUE_SIZE)
    {
        pthread_mutex_unlock(&queue.mutex);
        sched_yield();
        pthread_mutex_lock(&queue.mutex);
    }

    queue.data[queue.head] = value;

    queue.head =
        (queue.head + 1) % QUEUE_SIZE;

    queue.count++;

    pthread_mutex_unlock(&queue.mutex);
}

int pop(int* value)
{
    int result = 0;

    pthread_mutex_lock(&queue.mutex);

    if (queue.count > 0)
    {
        *value = queue.data[queue.tail];

        queue.tail =
            (queue.tail + 1) % QUEUE_SIZE;

        queue.count--;

        result = 1;
    }

    pthread_mutex_unlock(&queue.mutex);

    return result;
}

void* consumer(void* arg)
{
    int id = *(int*)arg;

    int consumed = 0;

    while (1)
    {
        int value;

        if (pop(&value))
        {
            consumed++;

            if (value == ITEMS - 1)
                break;
        }
        else
        {
            sched_yield();
        }
    }

    printf("Consumer %d consumed %d items\n",
        id,
        consumed);

    return NULL;
}

int main(void)
{
    queue.head = 0;
    queue.tail = 0;
    queue.count = 0;

    pthread_mutex_init(
        &queue.mutex,
        NULL
    );

    pthread_t consumers[CONSUMERS];

    int ids[CONSUMERS];

    for (int i = 0; i < CONSUMERS; i++)
    {
        ids[i] = i;

        pthread_create(
            &consumers[i],
            NULL,
            consumer,
            &ids[i]
        );
    }

    for (int i = 0; i < ITEMS; i++)
        push(i);

    for (int i = 0; i < CONSUMERS; i++)
        pthread_join(consumers[i], NULL);

    printf("SPMC completed\n");

    return 0;
}



