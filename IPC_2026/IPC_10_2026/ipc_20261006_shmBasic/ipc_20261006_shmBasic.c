/*
 * ipc_20261006_shmBasic.c
 *
 * Demonstrates:
 *     - shmget()
 *     - shmat()
 *     - writing data
 *     - reading data
 *     - shmdt()
 *     - shmctl()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// ---------------------------------------------------
#define SHM_FILE "/tmp/shm_basic_key"
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

    // ---------------------------------------------------
    // Write
    // ---------------------------------------------------
    strcpy(
        sharedMemory,
        "Hello from shared memory"
    );

    printf("Data written to shared memory\n");

    // ---------------------------------------------------
    // Read
    // ---------------------------------------------------
    printf(
        "Data read: %s\n",
        sharedMemory
    );

    // ---------------------------------------------------
    // Detach
    // ---------------------------------------------------
    shmdt(sharedMemory);

    // ---------------------------------------------------
    // Remove shared memory
    // ---------------------------------------------------
    shmctl(
        shmid,
        IPC_RMID,
        NULL
    );

    return EXIT_SUCCESS;
}



