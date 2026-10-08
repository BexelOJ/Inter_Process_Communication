// ---------------------------------------------------
// ipc_20261029_completeIPCSystem
// Pipe + shared memory + semaphore
// ---------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>

typedef struct
{
    char message[256];
} SharedData;

int main(void)
{
    int pipe_fd[2];

    pipe(pipe_fd);

    SharedData* shared =
        mmap(NULL,
            sizeof(SharedData),
            PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS,
            -1,
            0);

    if (shared == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    sem_t* sem =
        mmap(NULL,
            sizeof(sem_t),
            PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS,
            -1,
            0);

    sem_init(sem, 1, 0);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        close(pipe_fd[1]);

        char buffer[256];

        read(pipe_fd[0], buffer, sizeof(buffer));

        printf("Worker received through pipe: %s\n", buffer);

        snprintf(shared->message,
            sizeof(shared->message),
            "Worker processed: %s",
            buffer);

        sem_post(sem);

        close(pipe_fd[0]);

        exit(0);
    }

    close(pipe_fd[0]);

    const char* msg = "IPC request from parent";

    write(pipe_fd[1],
        msg,
        strlen(msg) + 1);

    close(pipe_fd[1]);

    sem_wait(sem);

    printf("Parent received from shared memory:\n");
    printf("%s\n", shared->message);

    wait(NULL);

    sem_destroy(sem);

    munmap(shared, sizeof(SharedData));
    munmap(sem, sizeof(sem_t));

    return 0;
}



