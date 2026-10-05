#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define WRITER_COUNT 3
#define INCREMENT_COUNT 10000

struct SharedData
{
    pthread_mutex_t mutex;
    int counter;
};

int main(void)
{
    int shmId;
    struct SharedData* sharedData;

    shmId = shmget(
        IPC_PRIVATE,
        sizeof(struct SharedData),
        IPC_CREAT | 0666
    );

    if (shmId == -1)
    {
        perror("shmget");
        return EXIT_FAILURE;
    }

    sharedData = shmat(shmId, NULL, 0);

    if (sharedData == (void*)-1)
    {
        perror("shmat");
        return EXIT_FAILURE;
    }

    pthread_mutexattr_t mutexAttr;

    pthread_mutexattr_init(&mutexAttr);

    pthread_mutexattr_setpshared(
        &mutexAttr,
        PTHREAD_PROCESS_SHARED
    );

    pthread_mutex_init(
        &sharedData->mutex,
        &mutexAttr
    );

    sharedData->counter = 0;

    for (int i = 0; i < WRITER_COUNT; i++)
    {
        pid_t processId = fork();

        if (processId == 0)
        {
            for (int j = 0; j < INCREMENT_COUNT; j++)
            {
                pthread_mutex_lock(
                    &sharedData->mutex
                );

                sharedData->counter++;

                pthread_mutex_unlock(
                    &sharedData->mutex
                );
            }

            shmdt(sharedData);

            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < WRITER_COUNT; i++)
    {
        wait(NULL);
    }

    printf("Final counter = %d\n",
        sharedData->counter);

    pthread_mutex_destroy(&sharedData->mutex);

    pthread_mutexattr_destroy(&mutexAttr);

    shmdt(sharedData);

    shmctl(shmId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


