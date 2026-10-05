/*
 * ipc_20261006_shmStruc.c
 *
 * Demonstrates:
 *     Structure stored in shared memory
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// ---------------------------------------------------
struct SharedData
{
    int id;
    float temperature;
    char name[64];
};

// ---------------------------------------------------
int main(void)
{
    int shmid;
    struct SharedData *data;

    shmid = shmget(
        IPC_PRIVATE,
        sizeof(struct SharedData),
        IPC_CREAT | 0666
    );

    if (shmid == -1)
    {
        perror("shmget");
        return EXIT_FAILURE;
    }

    data = (struct SharedData *)shmat(
        shmid,
        NULL,
        0
    );

    if (data == (struct SharedData *)-1)
    {
        perror("shmat");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Write structure
    // ---------------------------------------------------
    data->id = 101;
    data->temperature = 27.5;

    strcpy(
        data->name,
        "Temperature Sensor"
    );

    // ---------------------------------------------------
    // Read structure
    // ---------------------------------------------------
    printf("ID          : %d\n", data->id);
    printf("Temperature : %.2f\n", data->temperature);
    printf("Name        : %s\n", data->name);

    shmdt(data);

    shmctl(
        shmid,
        IPC_RMID,
        NULL
    );

    return EXIT_SUCCESS;
}



