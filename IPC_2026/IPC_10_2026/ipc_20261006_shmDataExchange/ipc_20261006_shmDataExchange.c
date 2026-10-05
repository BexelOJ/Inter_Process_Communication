/*
 * ipc_20261006_shmDataExchange.c
 *
 * Demonstrates:
 *     - Shared memory
 *     - fork()
 *     - Parent writes
 *     - Child reads
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

// ---------------------------------------------------
#define SHM_SIZE 1024

// ---------------------------------------------------
int main(void)
{
    int shmid;
    char *sharedMemory;
    pid_t pid;

    // ---------------------------------------------------
    // Create shared memory
    // ---------------------------------------------------
    shmid = shmget(
        IPC_PRIVATE,
        SHM_SIZE,
        IPC_CREAT | 0666
    );

    if (shmid == -1)
    {
        perror("shmget");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Attach
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

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Parent
    // ---------------------------------------------------
    if (pid > 0)
    {
        strcpy(
            sharedMemory,
            "Hello from parent"
        );

        printf("Parent wrote: %s\n", sharedMemory);

        wait(NULL);
    }

    // ---------------------------------------------------
    // Child
    // ---------------------------------------------------
    else
    {
        sleep(1);

        printf(
            "Child read: %s\n",
            sharedMemory
        );
    }

    // ---------------------------------------------------
    // Detach
    // ---------------------------------------------------
    shmdt(sharedMemory);

    // ---------------------------------------------------
    // Parent removes shared memory
    // ---------------------------------------------------
    if (pid > 0)
    {
        shmctl(
            shmid,
            IPC_RMID,
            NULL
        );
    }

    return EXIT_SUCCESS;
}



