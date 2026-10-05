#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void)
{
    int shmId;
    int sharedValue;
    void* sharedData;

    /* --------------------------------------------------- */
    /* Create shared memory                                */
    /* --------------------------------------------------- */

    shmId = shmget(IPC_PRIVATE,
        sizeof(int),
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

    sharedData = shmat(shmId, NULL, 0);

    if (sharedData == (void*)-1)
    {
        perror("shmat");
        shmctl(shmId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("Shared memory attached.\n");

    /* --------------------------------------------------- */
    /* Write                                              */
    /* --------------------------------------------------- */

    sharedValue = 1234;

    *(int*)sharedData = sharedValue;

    printf("Value written: %d\n",
        *(int*)sharedData);

    /* --------------------------------------------------- */
    /* Detach                                             */
    /* --------------------------------------------------- */

    if (shmdt(sharedData) == -1)
    {
        perror("shmdt");
        shmctl(shmId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("Shared memory detached using shmdt().\n");

    /* --------------------------------------------------- */
    /* Remove                                             */
    /* --------------------------------------------------- */

    if (shmctl(shmId, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        return EXIT_FAILURE;
    }

    printf("Shared memory removed.\n");

    return EXIT_SUCCESS;
}


