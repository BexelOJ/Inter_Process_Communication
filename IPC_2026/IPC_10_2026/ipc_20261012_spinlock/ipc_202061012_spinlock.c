#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define ITERATIONS 100000

pthread_spinlock_t spinlock;

int counter = 0;

void* worker(void* arg)
{
    int id = *(int*)arg;

    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_spin_lock(&spinlock);

        counter++;

        pthread_spin_unlock(&spinlock);
    }

    printf("Thread %d finished\n", id);

    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];

    int ids[NUM_THREADS];

    pthread_spin_init(&spinlock,
        PTHREAD_PROCESS_PRIVATE);

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

    printf("Counter = %d\n",
        counter);

    pthread_spin_destroy(&spinlock);

    return 0;
}


