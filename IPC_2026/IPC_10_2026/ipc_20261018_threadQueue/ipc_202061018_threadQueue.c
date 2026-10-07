#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define QUEUE_SIZE 10

typedef struct
{
    int data[QUEUE_SIZE];
    int head;
    int tail;
    int count;

    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;

} ThreadQueue;

ThreadQueue queue;

void queue_init(ThreadQueue* q)
{
    q->head = 0;
    q->tail = 0;
    q->count = 0;

    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

void queue_push(ThreadQueue* q, int value)
{
    pthread_mutex_lock(&q->mutex);

    while (q->count == QUEUE_SIZE)
        pthread_cond_wait(&q->not_full, &q->mutex);

    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    q->count++;

    pthread_cond_signal(&q->not_empty);

    pthread_mutex_unlock(&q->mutex);
}

int queue_pop(ThreadQueue* q)
{
    int value;

    pthread_mutex_lock(&q->mutex);

    while (q->count == 0)
        pthread_cond_wait(&q->not_empty, &q->mutex);

    value = q->data[q->head];

    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count--;

    pthread_cond_signal(&q->not_full);

    pthread_mutex_unlock(&q->mutex);

    return value;
}

void* producer(void* arg)
{
    for (int i = 1; i <= 20; i++)
    {
        queue_push(&queue, i);
        printf("Producer -> %d\n", i);
    }

    return NULL;
}

void* consumer(void* arg)
{
    for (int i = 1; i <= 20; i++)
    {
        int value = queue_pop(&queue);
        printf("Consumer <- %d\n", value);
    }

    return NULL;
}

int main(void)
{
    pthread_t producer_thread;
    pthread_t consumer_thread;

    queue_init(&queue);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}



