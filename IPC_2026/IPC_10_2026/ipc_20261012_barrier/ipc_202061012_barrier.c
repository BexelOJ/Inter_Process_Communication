#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4

pthread_barrier_t barrier;

void* worker(void* arg)
{
    int id = *(int*)arg;

    printf("Thread %d: Phase 1 started\n", id);

    sleep(id);

    printf("Thread %d: Reached barrier\n", id);

    pthread_barrier_wait(&barrier);

    printf("Thread %d: Passed barrier\n", id);

    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    pthread_barrier_init(&barrier,
        NULL,
        NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        ids[i] = i + 1;

        pthread_create(&threads[i],
            NULL,
            worker,
            &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    pthread_barrier_destroy(&barrier);

    return 0;
}



