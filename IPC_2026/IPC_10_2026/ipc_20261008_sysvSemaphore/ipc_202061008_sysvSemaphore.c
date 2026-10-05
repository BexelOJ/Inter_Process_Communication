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
    union semun argument;

    semId = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

    if (semId == -1)
    {
        perror("semget");
        return EXIT_FAILURE;
    }

    argument.value = 1;

    if (semctl(semId, 0, SETVAL, argument) == -1)
    {
        perror("semctl");
        semctl(semId, 0, IPC_RMID);
        return EXIT_FAILURE;
    }

    printf("System V Semaphore created.\n");
    printf("Semaphore ID: %d\n", semId);
    printf("Semaphore value: 1\n");

    if (semctl(semId, 0, IPC_RMID) == -1)
    {
        perror("semctl IPC_RMID");
        return EXIT_FAILURE;
    }

    printf("Semaphore removed.\n");

    return EXIT_SUCCESS;
}


