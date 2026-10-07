#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define THREADS 2

typedef struct
{
    int counter;

    pthread_mutex_t mutex;
    pthread_cond_t condition;

} SharedData;

SharedData* shared;

void* worker(void* arg)
{
    long id = (long)arg;

    for (int i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&shared->mutex);

        shared->counter++;

        printf("PID %d | Thread %ld | counter = %d\n",
            getpid(),
            id,
            shared->counter);

        pthread_cond_broadcast(&shared->condition);

        pthread_mutex_unlock(&shared->mutex);

        usleep(100000);
    }

    return NULL;
}

void start_threads(void)
{
    pthread_t threads[THREADS];

    for (long i = 0; i < THREADS; i++)
        pthread_create(&threads[i], NULL, worker, (void*)i);

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);
}

int main(void)
{
    shared = mmap(
        NULL,
        sizeof(SharedData),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );

    if (shared == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    shared->counter = 0;

    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    pthread_mutexattr_init(&mutex_attr);
    pthread_condattr_init(&cond_attr);

    pthread_mutexattr_setpshared(
        &mutex_attr,
        PTHREAD_PROCESS_SHARED
    );

    pthread_condattr_setpshared(
        &cond_attr,
        PTHREAD_PROCESS_SHARED
    );

    pthread_mutex_init(&shared->mutex, &mutex_attr);
    pthread_cond_init(&shared->condition, &cond_attr);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("\nChild process starting\n");

        start_threads();

        printf("Child process finished\n");

        return 0;
    }

    printf("Parent process starting\n");

    start_threads();

    waitpid(pid, NULL, 0);

    printf("\nFinal shared counter = %d\n",
        shared->counter);

    pthread_mutex_destroy(&shared->mutex);
    pthread_cond_destroy(&shared->condition);

    pthread_mutexattr_destroy(&mutex_attr);
    pthread_condattr_destroy(&cond_attr);

    munmap(shared, sizeof(SharedData));

    return 0;
}



