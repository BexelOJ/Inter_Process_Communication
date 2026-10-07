#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaphore;

void* worker(void* arg)
{
    printf("Worker: waiting for semaphore...\n");

    sem_wait(&semaphore);

    printf("Worker: semaphore acquired\n");

    sleep(2);

    printf("Worker: releasing semaphore\n");

    sem_post(&semaphore);

    return NULL;
}

int main(void)
{
    pthread_t t1;
    pthread_t t2;

    sem_init(&semaphore, 0, 1);

    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&semaphore);

    return 0;
}


