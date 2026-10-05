#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

int main(void)
{
    int msgId;
    int semId;
    int shmId;

    /* --------------------------------------------------- */
    /* Message Queue                                       */
    /* --------------------------------------------------- */

    msgId = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

    if (msgId == -1)
    {
        perror("msgget");
        return EXIT_FAILURE;
    }

    printf("Message Queue ID: %d\n", msgId);

    if (msgctl(msgId, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        return EXIT_FAILURE;
    }

    printf("Message Queue removed.\n");


    /* --------------------------------------------------- */
    /* Semaphore                                           */
    /* --------------------------------------------------- */

    semId = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

    if (semId == -1)
    {
        perror("semget");
        return EXIT_FAILURE;
    }

    printf("Semaphore ID: %d\n", semId);

    if (semctl(semId, 0, IPC_RMID) == -1)
    {
        perror("semctl");
        return EXIT_FAILURE;
    }

    printf("Semaphore removed.\n");


    /* --------------------------------------------------- */
    /* Shared Memory                                       */
    /* --------------------------------------------------- */

    shmId = shmget(IPC_PRIVATE, 1024, IPC_CREAT | 0666);

    if (shmId == -1)
    {
        perror("shmget");
        return EXIT_FAILURE;
    }

    printf("Shared Memory ID: %d\n", shmId);

    if (shmctl(shmId, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        return EXIT_FAILURE;
    }

    printf("Shared Memory removed.\n");

    return EXIT_SUCCESS;
}


