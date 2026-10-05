#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define ITEM_COUNT 10

struct RingBuffer
{
    int buffer[BUFFER_SIZE];

    int writeIndex;
    int readIndex;

    sem_t empty;
    sem_t full;
};

int main(void)
{
    int shmId;
    struct RingBuffer* ringBuffer;

    shmId = shmget(
        IPC_PRIVATE,
        sizeof(struct RingBuffer),
        IPC_CREAT | 0666
    );

    if (shmId == -1)
    {
        perror("shmget");
        return EXIT_FAILURE;
    }

    ringBuffer = shmat(shmId, NULL, 0);

    if (ringBuffer == (void*)-1)
    {
        perror("shmat");
        return EXIT_FAILURE;
    }

    ringBuffer->writeIndex = 0;
    ringBuffer->readIndex = 0;

    /*
     * empty = number of available slots
     * full  = number of available items
     */
    sem_init(
        &ringBuffer->empty,
        1,
        BUFFER_SIZE
    );

    sem_init(
        &ringBuffer->full,
        1,
        0
    );

    pid_t processId = fork();

    if (processId == -1)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (processId == 0)
    {
        /* Consumer */

        for (int i = 0; i < ITEM_COUNT; i++)
        {
            sem_wait(&ringBuffer->full);

            int value =
                ringBuffer->buffer[
                    ringBuffer->readIndex
                ];

            printf(
                "Consumer: %d from index %d\n",
                value,
                ringBuffer->readIndex
            );

            ringBuffer->readIndex =
                (ringBuffer->readIndex + 1)
                % BUFFER_SIZE;

            sem_post(&ringBuffer->empty);
        }

        shmdt(ringBuffer);

        exit(EXIT_SUCCESS);
    }

    /* Producer */

    for (int i = 0; i < ITEM_COUNT; i++)
    {
        sem_wait(&ringBuffer->empty);

        ringBuffer->buffer[
            ringBuffer->writeIndex
        ] = (i + 1) * 100;

        printf(
            "Producer: %d at index %d\n",
            ringBuffer->buffer[
                ringBuffer->writeIndex
            ],
            ringBuffer->writeIndex
        );

        ringBuffer->writeIndex =
            (ringBuffer->writeIndex + 1)
            % BUFFER_SIZE;

        sem_post(&ringBuffer->full);
    }

    wait(NULL);

    sem_destroy(&ringBuffer->empty);
    sem_destroy(&ringBuffer->full);

    shmdt(ringBuffer);

    shmctl(shmId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


