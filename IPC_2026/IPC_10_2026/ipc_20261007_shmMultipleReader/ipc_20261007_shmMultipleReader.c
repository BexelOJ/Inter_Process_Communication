#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

#define READER_COUNT 3

struct SharedData
{
    int value;
};

int main(void)
{
    int shmId;
    struct SharedData* sharedData;

    shmId = shmget(
        IPC_PRIVATE,
        sizeof(struct SharedData),
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
        return EXIT_FAILURE;
    }

    sharedData->value = 1000;

    printf("Shared value = %d\n",
        sharedData->value);

    for (int i = 0; i < READER_COUNT; i++)
    {
        pid_t processId = fork();

        if (processId == 0)
        {
            printf(
                "Reader %d: value = %d\n",
                i + 1,
                sharedData->value
            );

            shmdt(sharedData);

            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < READER_COUNT; i++)
    {
        wait(NULL);
    }

    shmdt(sharedData);

    shmctl(shmId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


