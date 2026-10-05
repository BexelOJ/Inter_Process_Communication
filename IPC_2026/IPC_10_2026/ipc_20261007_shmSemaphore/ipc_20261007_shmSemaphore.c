#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>

struct SharedData
{
    sem_t semaphore;

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

    /*
     * pshared = 1
     *
     * Semaphore is shared between processes.
     */
    if (sem_init(
        &sharedData->semaphore,
        1,
        1) == -1)
    {
        perror("sem_init");

        shmdt(sharedData);
        shmctl(shmId, IPC_RMID, NULL);

        return EXIT_FAILURE;
    }

    sharedData->value = 0;

    processId = fork();

    if (processId == 0)
    {
        printf("Child waiting for semaphore...\n");

        sem_wait(&sharedData->semaphore);

        printf("Child acquired semaphore.\n");

        sharedData->value = 100;

        printf(
            "Child changed value to %d\n",
            sharedData->value
        );

        sem_post(&sharedData->semaphore);

        printf("Child released semaphore.\n");

        shmdt(sharedData);

        exit(EXIT_SUCCESS);
    }

    wait(NULL);

    sem_wait(&sharedData->semaphore);

    printf(
        "Parent sees value = %d\n",
        sharedData->value
    );

    sem_post(&sharedData->semaphore);

    sem_destroy(&sharedData->semaphore);

    shmdt(sharedData);

    shmctl(shmId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


