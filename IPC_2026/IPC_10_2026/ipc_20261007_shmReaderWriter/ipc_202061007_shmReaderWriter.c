#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

struct SharedData
{
    pthread_rwlock_t lock;

    int value;
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

    pthread_rwlockattr_t lockAttr;

    pthread_rwlockattr_init(&lockAttr);

    pthread_rwlockattr_setpshared(
        &lockAttr,
        PTHREAD_PROCESS_SHARED
    );

    pthread_rwlock_init(
        &sharedData->lock,
        &lockAttr
    );

    sharedData->value = 100;

    /*
     * Reader
     */
    pid_t reader = fork();

    if (reader == 0)
    {
        pthread_rwlock_rdlock(
            &sharedData->lock
        );

        printf(
            "Reader: value = %d\n",
            sharedData->value
        );

        sleep(2);

        pthread_rwlock_unlock(
            &sharedData->lock
        );

        shmdt(sharedData);

        exit(EXIT_SUCCESS);
    }

    /*
     * Writer
     */
    pid_t writer = fork();

    if (writer == 0)
    {
        sleep(1);

        printf("Writer waiting for lock...\n");

        pthread_rwlock_wrlock(
            &sharedData->lock
        );

        sharedData->value = 500;

        printf(
            "Writer changed value to %d\n",
            sharedData->value
        );

        pthread_rwlock_unlock(
            &sharedData->lock
        );

        shmdt(sharedData);

        exit(EXIT_SUCCESS);
    }

    wait(NULL);
    wait(NULL);

    pthread_rwlock_destroy(
        &sharedData->lock
    );

    pthread_rwlockattr_destroy(&lockAttr);

    shmdt(sharedData);

    shmctl(shmId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


