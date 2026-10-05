#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/wait.h>

struct SharedData
{
    pthread_mutex_t mutex;
    pthread_cond_t condition;

    int dataReady;
    int value;
};

int main(void)
{
    int shmId;
    struct SharedData* sharedData;
    pid_t processId;

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
        shmctl(shmId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    pthread_mutexattr_t mutexAttr;
    pthread_condattr_t condAttr;

    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_setpshared(
        &mutexAttr,
        PTHREAD_PROCESS_SHARED
    );

    pthread_condattr_init(&condAttr);
    pthread_condattr_setpshared(
        &condAttr,
        PTHREAD_PROCESS_SHARED
    );

    pthread_mutex_init(
        &sharedData->mutex,
        &mutexAttr
    );

    pthread_cond_init(
        &sharedData->condition,
        &condAttr
    );

    sharedData->dataReady = 0;
    sharedData->value = 0;

    processId = fork();

    if (processId == -1)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (processId == 0)
    {
        /* Consumer */

        pthread_mutex_lock(&sharedData->mutex);

        while (sharedData->dataReady == 0)
        {
            printf("Consumer: waiting...\n");

            pthread_cond_wait(
                &sharedData->condition,
                &sharedData->mutex
            );
        }

        printf(
            "Consumer: received %d\n",
            sharedData->value
        );

        pthread_mutex_unlock(&sharedData->mutex);

        shmdt(sharedData);

        exit(EXIT_SUCCESS);
    }

    /* Producer */

    sleep(2);

    pthread_mutex_lock(&sharedData->mutex);

    sharedData->value = 500;
    sharedData->dataReady = 1;

    printf("Producer: produced %d\n",
        sharedData->value);

    pthread_cond_signal(
        &sharedData->condition
    );

    pthread_mutex_unlock(&sharedData->mutex);

    wait(NULL);

    pthread_cond_destroy(&sharedData->condition);
    pthread_mutex_destroy(&sharedData->mutex);

    pthread_condattr_destroy(&condAttr);
    pthread_mutexattr_destroy(&mutexAttr);

    shmdt(sharedData);

    shmctl(shmId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


