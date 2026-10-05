/*
 * ipc_20261006_shmAttach.c
 *
 * Demonstrates:
 *     - shmget()
 *     - shmat()
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// ---------------------------------------------------
#define SHM_FILE "/tmp/shm_attach_key"
#define SHM_SIZE 1024

// ---------------------------------------------------
int main(void)
{
    key_t key;
    int shmid;
    char *sharedMemory;

    // ---------------------------------------------------
    // Generate key
    // ---------------------------------------------------
    key = ftok(SHM_FILE, 'A');

    if (key == -1)
    {
        perror("ftok");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Get/create shared memory
    // ---------------------------------------------------
    shmid = shmget(
        key,
        SHM_SIZE,
        IPC_CREAT | 0666
    );

    if (shmid == -1)
    {
        perror("shmget");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Attach shared memory
    // ---------------------------------------------------
    sharedMemory = (char *)shmat(
        shmid,
        NULL,
        0
    );

    if (sharedMemory == (char *)-1)
    {
        perror("shmat");
        return EXIT_FAILURE;
    }

    printf("Shared memory attached\n");
    printf("Address: %p\n", (void *)sharedMemory);

    // ---------------------------------------------------
    // Detach
    // ---------------------------------------------------
    shmdt(sharedMemory);

    return EXIT_SUCCESS;
}



