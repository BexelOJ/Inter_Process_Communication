#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>

struct SharedData
{
    sem_t empty;
    sem_t full;

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

    sem_init(&sharedData->empty, 1, 1);
    sem_init(&sharedData->full, 1, 0);

    processId = fork();

    if (processId == 0)
    {
        /* Consumer */

        sem_wait(&sharedData->full);

        printf(
            "Consumer received: %d\n",
            sharedData->value
        );

        sem_post(&sharedData->empty);

        shmdt(sharedData);

        exit(EXIT_SUCCESS);
    }

    /* Producer */

    sem_wait(&sharedData->empty);

    sharedData->value = 500;

    printf(
        "Producer produced: %d\n",
        sharedData->value
    );

    sem_post(&sharedData->full);

    wait(NULL);

    sem_destroy(&sharedData->empty);
    sem_destroy(&sharedData->full);

    shmdt(sharedData);

    shmctl(shmId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


