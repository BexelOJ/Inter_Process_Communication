#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>

#define SHM_NAME "/ipc_process_mutex"

struct SharedData
{
    pthread_mutex_t mutex;
    int counter;
};

int main(void)
{
    int shmFd;

    struct SharedData* shared;

    shm_unlink(SHM_NAME);

    shmFd = shm_open(SHM_NAME,
        O_CREAT | O_RDWR,
        0666);

    if (shmFd < 0)
    {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    ftruncate(shmFd,
        sizeof(struct SharedData));

    shared = mmap(NULL,
        sizeof(struct SharedData),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        shmFd,
        0);

    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);

    pthread_mutexattr_setpshared(
        &attr,
        PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&shared->mutex,
        &attr);

    shared->counter = 0;

    pid_t pid = fork();

    if (pid == 0)
    {
        for (int i = 0; i < 100000; i++)
        {
            pthread_mutex_lock(&shared->mutex);

            shared->counter++;

            pthread_mutex_unlock(&shared->mutex);
        }

        munmap(shared,
            sizeof(struct SharedData));

        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < 100000; i++)
    {
        pthread_mutex_lock(&shared->mutex);

        shared->counter++;

        pthread_mutex_unlock(&shared->mutex);
    }

    wait(NULL);

    printf("Final counter = %d\n",
        shared->counter);

    pthread_mutex_destroy(&shared->mutex);

    pthread_mutexattr_destroy(&attr);

    munmap(shared,
        sizeof(struct SharedData));

    close(shmFd);

    shm_unlink(SHM_NAME);

    return EXIT_SUCCESS;
}


