#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define QUEUE_SIZE 16
#define WORKERS 4
#define JOBS 20

typedef struct
{
    int data[QUEUE_SIZE];

    int head;
    int tail;
    int count;

    pthread_mutex_t mutex;
    pthread_cond_t condition;

} JobQueue;

JobQueue queue;

void enqueue(int value)
{
    pthread_mutex_lock(&queue.mutex);

    while (queue.count == QUEUE_SIZE)
        pthread_cond_wait(&queue.condition, &queue.mutex);

    queue.data[queue.tail] = value;
    queue.tail = (queue.tail + 1) % QUEUE_SIZE;
    queue.count++;

    pthread_cond_broadcast(&queue.condition);

    pthread_mutex_unlock(&queue.mutex);
}

int dequeue(void)
{
    int value;

    pthread_mutex_lock(&queue.mutex);

    while (queue.count == 0)
        pthread_cond_wait(&queue.condition, &queue.mutex);

    value = queue.data[queue.head];

    queue.head = (queue.head + 1) % QUEUE_SIZE;
    queue.count--;

    pthread_cond_broadcast(&queue.condition);

    pthread_mutex_unlock(&queue.mutex);

    return value;
}

void* worker(void* arg)
{
    long id = (long)arg;

    for (;;)
    {
        int job = dequeue();

        if (job == -1)
            break;

        printf("Worker %ld processing job %d\n", id, job);
    }

    return NULL;
}

int main(void)
{
    pthread_t workers[WORKERS];

    pthread_mutex_init(&queue.mutex, NULL);
    pthread_cond_init(&queue.condition, NULL);

    for (long i = 0; i < WORKERS; i++)
        pthread_create(&workers[i], NULL, worker, (void*)i);

    for (int i = 1; i <= JOBS; i++)
        enqueue(i);

    for (int i = 0; i < WORKERS; i++)
        enqueue(-1);

    for (int i = 0; i < WORKERS; i++)
        pthread_join(workers[i], NULL);

    pthread_mutex_destroy(&queue.mutex);
    pthread_cond_destroy(&queue.condition);

    return 0;
}


