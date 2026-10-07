#include <stdio.h>
#include <pthread.h>

typedef struct
{
    int counter;
    pthread_mutex_t mutex;

} SharedData;

SharedData shared;

void* worker(void* arg)
{
    for (int i = 0; i < 100000; i++)
    {
        pthread_mutex_lock(&shared.mutex);

        shared.counter++;

        pthread_mutex_unlock(&shared.mutex);
    }

    return NULL;
}

int main(void)
{
    pthread_t t1;
    pthread_t t2;

    shared.counter = 0;

    pthread_mutex_init(&shared.mutex, NULL);

    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter = %d\n", shared.counter);

    pthread_mutex_destroy(&shared.mutex);

    return 0;
}



