#include <stdio.h>
#include <pthread.h>

#define QUEUE_SIZE 128
#define PRODUCERS 4
#define ITEMS 1000

struct Queue
{
    int data[QUEUE_SIZE];

    int head;
    int tail;
    int count;

    pthread_mutex_t mutex;
};

struct Queue queue;

void queue_init(void)
{
    queue.head = 0;
    queue.tail = 0;
    queue.count = 0;

    pthread_mutex_init(&queue.mutex, NULL);
}

void queue_push(int value)
{
    pthread_mutex_lock(&queue.mutex);

    if (queue.count < QUEUE_SIZE)
    {
        queue.data[queue.head] = value;

        queue.head =
            (queue.head + 1) % QUEUE_SIZE;

        queue.count++;
    }

    pthread_mutex_unlock(&queue.mutex);
}

int queue_pop(int* value)
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

void* producer(void* arg)
{
    int id = *(int*)arg;

    for (int i = 0; i < ITEMS; i++)
    {
        queue_push(id * ITEMS + i);
    }

    return NULL;
}

int main(void)
{
    queue_init();

    pthread_t threads[PRODUCERS];

    int ids[PRODUCERS];

    for (int i = 0; i < PRODUCERS; i++)
    {
        ids[i] = i;

        pthread_create(
            &threads[i],
            NULL,
            producer,
            &ids[i]
        );
    }

    int consumed = 0;

    while (consumed < PRODUCERS * ITEMS)
    {
        int value;

        if (queue_pop(&value))
        {
            consumed++;

            if (consumed % 500 == 0)
                printf("Consumed %d items\n",
                    consumed);
        }
    }

    for (int i = 0; i < PRODUCERS; i++)
        pthread_join(threads[i], NULL);

    printf("MPSC complete: %d items\n", consumed);

    return 0;
}



