#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>

#define SHM_NAME "/ipc_process_condition"

struct SharedData
{
    pthread_mutex_t mutex;
    pthread_cond_t condition;

    int ready;
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

    ftruncate(shmFd, sizeof(struct SharedData));

    shared = mmap(NULL,
        sizeof(struct SharedData),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        shmFd,
        0);

    if (shared == MAP_FAILED)
    {
        perror("mmap");
        return EXIT_FAILURE;
    }

    pthread_mutexattr_t mutexAttr;
    pthread_condattr_t condAttr;

    pthread_mutexattr_init(&mutexAttr);
    pthread_condattr_init(&condAttr);

    pthread_mutexattr_setpshared(
        &mutexAttr,
        PTHREAD_PROCESS_SHARED);

    pthread_condattr_setpshared(
        &condAttr,
        PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&shared->mutex,
        &mutexAttr);

    pthread_cond_init(&shared->condition,
        &condAttr);

    shared->ready = 0;

    pid_t pid = fork();

    if (pid == 0)
    {
        printf("Child: waiting for condition...\n");

        pthread_mutex_lock(&shared->mutex);

        while (!shared->ready)
        {
            pthread_cond_wait(
                &shared->condition,
                &shared->mutex);
        }

        printf("Child: condition received!\n");

        pthread_mutex_unlock(&shared->mutex);

        munmap(shared,
            sizeof(struct SharedData));

        exit(EXIT_SUCCESS);
    }

    sleep(2);

    printf("Parent: setting condition...\n");

    pthread_mutex_lock(&shared->mutex);

    shared->ready = 1;

    pthread_cond_signal(&shared->condition);

    pthread_mutex_unlock(&shared->mutex);

    wait(NULL);

    pthread_cond_destroy(&shared->condition);
    pthread_mutex_destroy(&shared->mutex);

    pthread_mutexattr_destroy(&mutexAttr);
    pthread_condattr_destroy(&condAttr);

    munmap(shared,
        sizeof(struct SharedData));

    close(shmFd);

    shm_unlink(SHM_NAME);

    return EXIT_SUCCESS;
}



