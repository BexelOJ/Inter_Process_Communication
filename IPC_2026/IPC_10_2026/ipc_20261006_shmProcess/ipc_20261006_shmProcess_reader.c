/*
 * ipc_20261006_shmProcess_reader.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// ---------------------------------------------------
#define SHM_FILE "/tmp/shm_process_key"
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
        0666
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

    printf(
        "Reader: %s\n",
        sharedMemory
    );

    shmdt(sharedMemory);

    shmctl(
        shmid,
        IPC_RMID,
        NULL
    );

    return EXIT_SUCCESS;
}



