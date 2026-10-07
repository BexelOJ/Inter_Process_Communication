#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t rwlock =
PTHREAD_RWLOCK_INITIALIZER;

int sharedData = 0;

void* reader(void* arg)
{
    int id = *(int*)arg;

    pthread_rwlock_rdlock(&rwlock);

    printf("Reader %d: data = %d\n",
        id,
        sharedData);

    sleep(1);

    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

void* writer(void* arg)
{
    int id = *(int*)arg;

    pthread_rwlock_wrlock(&rwlock);

    sharedData++;

    printf("Writer %d: data = %d\n",
        id,
        sharedData);

    sleep(2);

    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

int main(void)
{
    pthread_t readers[3];
    pthread_t writers[2];

    int readerIds[3] = { 1, 2, 3 };
    int writerIds[2] = { 1, 2 };

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&readers[i],
            NULL,
            reader,
            &readerIds[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        pthread_create(&writers[i],
            NULL,
            writer,
            &writerIds[i]);
    }

    for (int i = 0; i < 3; i++)
        pthread_join(readers[i], NULL);

    for (int i = 0; i < 2; i++)
        pthread_join(writers[i], NULL);

    pthread_rwlock_destroy(&rwlock);

    return 0;
}



