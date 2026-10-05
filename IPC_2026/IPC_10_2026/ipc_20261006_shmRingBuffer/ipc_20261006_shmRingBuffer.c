#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 5

struct RingBuffer
{
    int buffer[BUFFER_SIZE];

    int writeIndex;
    int readIndex;
};

int main(void)
{
    int shmId;

    struct RingBuffer *ringBuffer;

    /*
     * Create shared memory.
     */
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

    /*
     * Attach shared memory.
     */
    ringBuffer = (struct RingBuffer *)shmat(
        shmId,
        NULL,
        0
    );

    if (ringBuffer == (void *)-1)
    {
        perror("shmat");
        shmctl(shmId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    /*
     * Initialize indexes.
     */
    ringBuffer->writeIndex = 0;
    ringBuffer->readIndex = 0;

    /*
     * Producer.
     */
    printf("Producing data:\n");

    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        ringBuffer->buffer[ringBuffer->writeIndex] =
            (i + 1) * 100;

        printf(
            "Produced: %d at index %d\n",
            ringBuffer->buffer[ringBuffer->writeIndex],
            ringBuffer->writeIndex
        );

        ringBuffer->writeIndex =
            (ringBuffer->writeIndex + 1) % BUFFER_SIZE;
    }

    /*
     * Consumer.
     */
    printf("\nConsuming data:\n");

    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        printf(
            "Consumed: %d from index %d\n",
            ringBuffer->buffer[ringBuffer->readIndex],
            ringBuffer->readIndex
        );

        ringBuffer->readIndex =
            (ringBuffer->readIndex + 1) % BUFFER_SIZE;
    }

    /*
     * Detach.
     */
    if (shmdt(ringBuffer) == -1)
    {
        perror("shmdt");
    }

    /*
     * Remove shared memory.
     */
    if (shmctl(shmId, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}



