#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

struct SharedData
{
    pthread_mutex_t mutex;
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

    sharedData->value = 0;

    processId = fork();

    if (processId == 0)
    {
        pthread_mutex_lock(&sharedData->mutex);

        printf("Child: locked mutex\n");

        sharedData->value = 100;

        printf("Child: value = %d\n",
            sharedData->value);

        pthread_mutex_unlock(&sharedData->mutex);

        printf("Child: unlocked mutex\n");

        shmdt(sharedData);

        exit(EXIT_SUCCESS);
    }

    wait(NULL);

    pthread_mutex_lock(&sharedData->mutex);

    printf("Parent: value = %d\n",
        sharedData->value);

    pthread_mutex_unlock(&sharedData->mutex);

    pthread_mutex_destroy(&sharedData->mutex);

    pthread_mutexattr_destroy(&mutexAttr);

    shmdt(sharedData);

    shmctl(shmId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


