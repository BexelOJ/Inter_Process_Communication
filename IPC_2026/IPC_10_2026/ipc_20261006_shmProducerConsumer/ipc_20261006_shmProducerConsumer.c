#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

struct SharedData
{
    int ready;
    int value;
};

int main(void)
{
    int shmId;
    struct SharedData *sharedData;
    pid_t processId;

    /*
     * Create shared memory.
     */
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

    /*
     * Attach shared memory.
     */
    sharedData = (struct SharedData *)shmat(
        shmId,
        NULL,
        0
    );

    if (sharedData == (void *)-1)
    {
        perror("shmat");
        shmctl(shmId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    sharedData->ready = 0;
    sharedData->value = 0;

    processId = fork();

    if (processId == -1)
    {
        perror("fork");

        shmdt(sharedData);
        shmctl(shmId, IPC_RMID, NULL);

        return EXIT_FAILURE;
    }

    if (processId == 0)
    {
        /*
         * Child = Consumer
         */
        printf("Consumer: waiting for data...\n");

        while (sharedData->ready == 0)
        {
            usleep(10000);
        }

        printf("Consumer: received value = %d\n",
               sharedData->value);

        shmdt(sharedData);

        exit(EXIT_SUCCESS);
    }
    else
    {
        /*
         * Parent = Producer
         */
        printf("Producer: preparing data...\n");

        sleep(2);

        sharedData->value = 100;

        /*
         * Tell consumer that data is ready.
         */
        sharedData->ready = 1;

        printf("Producer: produced value = %d\n",
               sharedData->value);

        wait(NULL);

        shmdt(sharedData);

        /*
         * Remove shared memory.
         */
        shmctl(shmId, IPC_RMID, NULL);

        printf("Producer: shared memory removed.\n");
    }

    return EXIT_SUCCESS;
}



