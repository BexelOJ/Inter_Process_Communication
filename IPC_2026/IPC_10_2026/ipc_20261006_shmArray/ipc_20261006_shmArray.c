/*
 * ipc_20261006_shmArray.c
 *
 * Demonstrates:
 *     Array stored in shared memory
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// ---------------------------------------------------
#define ARRAY_SIZE 10

// ---------------------------------------------------
int main(void)
{
    int shmid;
    int *sharedArray;

    shmid = shmget(
        IPC_PRIVATE,
        ARRAY_SIZE * sizeof(int),
        IPC_CREAT | 0666
    );

    if (shmid == -1)
    {
        perror("shmget");
        return EXIT_FAILURE;
    }

    sharedArray = (int *)shmat(
        shmid,
        NULL,
        0
    );

    if (sharedArray == (int *)-1)
    {
        perror("shmat");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Write array
    // ---------------------------------------------------
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        sharedArray[i] = i * 10;
    }

    // ---------------------------------------------------
    // Read array
    // ---------------------------------------------------
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        printf(
            "sharedArray[%d] = %d\n",
            i,
            sharedArray[i]
        );
    }

    shmdt(sharedArray);

    shmctl(
        shmid,
        IPC_RMID,
        NULL
    );

    return EXIT_SUCCESS;
}



