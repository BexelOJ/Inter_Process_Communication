#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

#define SEM_NAME "/ipc_named_sem"

int main(void)
{
    sem_t* sem;

    sem = sem_open(SEM_NAME,
        O_CREAT,
        0666,
        1);

    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        return EXIT_FAILURE;
    }

    printf("Waiting for semaphore...\n");

    sem_wait(sem);

    printf("Semaphore acquired.\n");

    printf("Critical section...\n");

    sleep(3);

    printf("Releasing semaphore.\n");

    sem_post(sem);

    sem_close(sem);

    sem_unlink(SEM_NAME);

    return EXIT_SUCCESS;
}



