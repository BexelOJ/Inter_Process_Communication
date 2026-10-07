#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int data_ready = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

void* consumer(void* arg)
{
    pthread_mutex_lock(&mutex);

    while (!data_ready)
    {
        printf("Consumer: waiting for data...\n");

        pthread_cond_wait(&condition, &mutex);
    }

    printf("Consumer: data received\n");

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* producer(void* arg)
{
    sleep(2);

    pthread_mutex_lock(&mutex);

    data_ready = 1;

    printf("Producer: data is ready\n");

    pthread_cond_signal(&condition);

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void)
{
    pthread_t producer_thread;
    pthread_t consumer_thread;

    pthread_create(&consumer_thread, NULL, consumer, NULL);
    pthread_create(&producer_thread, NULL, producer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);

    return 0;
}



