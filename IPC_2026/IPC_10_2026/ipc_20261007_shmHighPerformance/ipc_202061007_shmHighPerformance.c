#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define DATA_SIZE (1024 * 1024)

int main(void)
{
    int shmId;
    char* sharedData;

    shmId = shmget(
        IPC_PRIVATE,
        DATA_SIZE,
        IPC_CREAT | 0666
    );

    if (shmId == -1)
    {
        perror("shmget");
        return EXIT_FAILURE;
    }

    sharedData = shmat(shmId, NULL, 0);

    if (sharedData == (void*)-1)
    {
        perror("shmat");
        shmctl(shmId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("Writing 1 MB into shared memory...\n");

    memset(sharedData, 'A', DATA_SIZE);

    printf("Data written successfully.\n");

    printf("First byte : %c\n",
        sharedData[0]);

    printf("Last byte  : %c\n",
        sharedData[DATA_SIZE - 1]);

    shmdt(sharedData);

    shmctl(shmId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}



