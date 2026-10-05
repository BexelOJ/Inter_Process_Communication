/*
 * ipc_20261006_shmDetach.c
 *
 * Demonstrates:
 *     - shmat()
 *     - shmdt()
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// ---------------------------------------------------
#define SHM_FILE "/tmp/shm_detach_key"
#define SHM_SIZE 1024

// ---------------------------------------------------
int main(void)
{
    key_t key;
    int shmid;
    char *sharedMemory;

    key = ftok(SHM_FILE, 'A');

    if (key == -1)
    {
        perror("ftok");
        return EXIT_FAILURE;
    }

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

    // ---------------------------------------------------
    // Detach
    // ---------------------------------------------------
    if (shmdt(sharedMemory) == -1)
    {
        perror("shmdt");
        return EXIT_FAILURE;
    }

    printf("Shared memory detached\n");

    return EXIT_SUCCESS;
}



