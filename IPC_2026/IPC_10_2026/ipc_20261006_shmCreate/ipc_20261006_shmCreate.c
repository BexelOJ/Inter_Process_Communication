/*
 * ipc_20261006_shmCreate.c
 *
 * Demonstrates:
 *     - ftok()
 *     - shmget()
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

// ---------------------------------------------------
#define SHM_FILE "/tmp/shm_create_key"

#define SHM_SIZE 1024

// ---------------------------------------------------
int main(void)
{
    key_t key;
    int shmid;

    // ---------------------------------------------------
    // Create key
    // ---------------------------------------------------
    key = ftok(SHM_FILE, 'A');

    if (key == -1)
    {
        perror("ftok");
        return EXIT_FAILURE;
    }

    printf("Key created: %d\n", key);

    // ---------------------------------------------------
    // Create shared memory
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

    printf("Shared memory created\n");
    printf("SHM ID: %d\n", shmid);

    return EXIT_SUCCESS;
}



