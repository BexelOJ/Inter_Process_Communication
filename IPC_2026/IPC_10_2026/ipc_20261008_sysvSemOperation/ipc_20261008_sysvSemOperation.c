#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun
{
    int value;
    struct semid_ds* buffer;
    unsigned short* array;
};

int main(void)
{
    int semId;
    struct sembuf operation;
    union semun argument;

    /* --------------------------------------------------- */
    /* Create semaphore                                    */
    /* --------------------------------------------------- */

    semId = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

    if (semId == -1)
    {
        perror("semget");
        return EXIT_FAILURE;
    }

    /* --------------------------------------------------- */
    /* Initialize semaphore to 1                          */
    /* --------------------------------------------------- */

    argument.value = 1;

    if (semctl(semId, 0, SETVAL, argument) == -1)
    {
        perror("semctl");
        semctl(semId, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    printf("Semaphore initialized to 1.\n");

    /* --------------------------------------------------- */
    /* P operation / Wait                                  */
    /* --------------------------------------------------- */

    operation.sem_num = 0;
    operation.sem_op = -1;
    operation.sem_flg = 0;

    if (semop(semId, &operation, 1) == -1)
    {
        perror("semop wait");
        semctl(semId, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    printf("Semaphore acquired.\n");

    /* --------------------------------------------------- */
    /* V operation / Signal                                */
    /* --------------------------------------------------- */

    operation.sem_num = 0;
    operation.sem_op = 1;
    operation.sem_flg = 0;

    if (semop(semId, &operation, 1) == -1)
    {
        perror("semop signal");
        semctl(semId, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    printf("Semaphore released.\n");

    semctl(semId, 0, IPC_RMID);

    return EXIT_SUCCESS;
}


