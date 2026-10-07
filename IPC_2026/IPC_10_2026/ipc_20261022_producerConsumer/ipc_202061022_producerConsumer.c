#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

#define SHM_NAME "/producer_consumer_shm"

typedef struct
{
    int value;

    sem_t empty;
    sem_t full;

} SharedData;

int main(void)
{
    int shm_fd;

    shm_fd = shm_open(SHM_NAME,
        O_CREAT | O_RDWR,
        0666);

    ftruncate(shm_fd, sizeof(SharedData));

    SharedData* data =
        mmap(NULL,
            sizeof(SharedData),
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            shm_fd,
            0);

    sem_init(&data->empty, 1, 1);
    sem_init(&data->full, 1, 0);

    pid_t pid = fork();

    if (pid == 0)
    {
        for (int i = 1; i <= 5; i++)
        {
            sem_wait(&data->empty);

            data->value = i;

            printf("Producer: %d\n", i);

            sem_post(&data->full);

            sleep(1);
        }

        exit(EXIT_SUCCESS);
    }

    for (int i = 1; i <= 5; i++)
    {
        sem_wait(&data->full);

        printf("Consumer: %d\n", data->value);

        sem_post(&data->empty);
    }

    wait(NULL);

    sem_destroy(&data->empty);
    sem_destroy(&data->full);

    munmap(data, sizeof(SharedData));

    close(shm_fd);

    shm_unlink(SHM_NAME);

    return EXIT_SUCCESS;
}



