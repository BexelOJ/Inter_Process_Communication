#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define WORKERS 3
#define JOBS 10

typedef struct
{
    int job;
    int shutdown;

} WorkerMessage;

WorkerMessage messages[JOBS + WORKERS];

int head = 0;
int tail = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

void submit_job(int job)
{
    pthread_mutex_lock(&mutex);

    messages[tail].job = job;
    messages[tail].shutdown = 0;

    tail++;

    pthread_cond_signal(&condition);

    pthread_mutex_unlock(&mutex);
}

void shutdown_worker(void)
{
    pthread_mutex_lock(&mutex);

    messages[tail].shutdown = 1;

    tail++;

    pthread_cond_signal(&condition);

    pthread_mutex_unlock(&mutex);
}

void* worker(void* arg)
{
    long id = (long)arg;

    while (1)
    {
        pthread_mutex_lock(&mutex);

        while (head == tail)
            pthread_cond_wait(&condition, &mutex);

        WorkerMessage message = messages[head++];

        pthread_mutex_unlock(&mutex);

        if (message.shutdown)
        {
            printf("Worker %ld shutting down\n", id);
            break;
        }

        printf("Worker %ld processing job %d\n",
            id, message.job);

        usleep(200000);
    }

    return NULL;
}

int main(void)
{
    pthread_t workers[WORKERS];

    for (long i = 0; i < WORKERS; i++)
        pthread_create(&workers[i], NULL, worker, (void*)i);

    for (int i = 1; i <= JOBS; i++)
        submit_job(i);

    for (int i = 0; i < WORKERS; i++)
        shutdown_worker();

    for (int i = 0; i < WORKERS; i++)
        pthread_join(workers[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);

    return 0;
}



