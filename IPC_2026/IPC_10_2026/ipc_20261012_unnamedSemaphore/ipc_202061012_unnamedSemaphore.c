#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaphore;

void* worker(void* arg)
{
    int id = *(int*)arg;

    printf("Thread %d waiting...\n", id);

    sem_wait(&semaphore);

    printf("Thread %d entered critical section\n",
        id);

    sleep(2);

    printf("Thread %d leaving\n",
        id);

    sem_post(&semaphore);

    return NULL;
}

int main(void)
{
    pthread_t threads[3];

    int ids[3] = { 1, 2, 3 };

    /*
     * pshared = 0
     *
     * Semaphore is shared between threads
     * of this process.
     */
    sem_init(&semaphore,
        0,
        1);

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&threads[i],
            NULL,
            worker,
            &ids[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i],
            NULL);
    }

    sem_destroy(&semaphore);

    return 0;
}



