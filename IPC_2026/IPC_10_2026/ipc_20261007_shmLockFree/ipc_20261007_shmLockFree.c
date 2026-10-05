#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdatomic.h>

struct SharedData
{
    atomic_int counter;
};

int main(void)
{
    int shmId;
    struct SharedData* sharedData;
    pid_t processId;

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
        shmctl(shmId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    atomic_init(&sharedData->counter, 0);

    processId = fork();

    if (processId == -1)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (processId == 0)
    {
        for (int i = 0; i < 100000; i++)
        {
            atomic_fetch_add(
                &sharedData->counter,
                1
            );
        }

        shmdt(sharedData);
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < 100000; i++)
    {
        atomic_fetch_add(
            &sharedData->counter,
            1
        );
    }

    wait(NULL);

    printf("Final counter = %d\n",
        atomic_load(&sharedData->counter));

    shmdt(sharedData);

    shmctl(shmId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


