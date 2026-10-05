#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void)
{
    int shmId;
    char* sharedData;

    shmId = shmget(IPC_PRIVATE,
        1024,
        IPC_CREAT | 0666);

    if (shmId == -1)
    {
        perror("shmget");
        return EXIT_FAILURE;
    }

    printf("Shared Memory ID: %d\n", shmId);

    /* --------------------------------------------------- */
    /* Attach shared memory                                */
    /* --------------------------------------------------- */

    sharedData = (char*)shmat(shmId, NULL, 0);

    if (sharedData == (char*)-1)
    {
        perror("shmat");
        shmctl(shmId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("Shared memory attached successfully.\n");

    strcpy(sharedData, "Data written after shmat().");

    printf("Shared Data: %s\n", sharedData);

    /* --------------------------------------------------- */
    /* Detach                                              */
    /* --------------------------------------------------- */

    if (shmdt(sharedData) == -1)
    {
        perror("shmdt");
        shmctl(shmId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("Shared memory detached.\n");

    shmctl(shmId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


