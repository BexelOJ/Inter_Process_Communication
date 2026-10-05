#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void)
{
    int shmId;
    char* sharedData;

    /* --------------------------------------------------- */
    /* Create shared memory                                */
    /* --------------------------------------------------- */

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
    /* Attach                                             */
    /* --------------------------------------------------- */

    sharedData = (char*)shmat(shmId, NULL, 0);

    if (sharedData == (char*)-1)
    {
        perror("shmat");
        shmctl(shmId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    /* --------------------------------------------------- */
    /* Write                                               */
    /* --------------------------------------------------- */

    strcpy(sharedData,
        "Hello from System V Shared Memory");

    printf("Data written to shared memory.\n");

    /* --------------------------------------------------- */
    /* Read                                                */
    /* --------------------------------------------------- */

    printf("Data: %s\n", sharedData);

    /* --------------------------------------------------- */
    /* Detach                                              */
    /* --------------------------------------------------- */

    if (shmdt(sharedData) == -1)
    {
        perror("shmdt");
    }

    /* --------------------------------------------------- */
    /* Remove                                              */
    /* --------------------------------------------------- */

    if (shmctl(shmId, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        return EXIT_FAILURE;
    }

    printf("Shared memory removed.\n");

    return EXIT_SUCCESS;
}


