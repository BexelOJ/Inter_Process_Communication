#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#define THREADS 4

void* worker(void* arg)
{
    long id = (long)arg;

    printf("PID %d -> Worker thread %ld started\n",
        getpid(), id);

    sleep(2);

    printf("PID %d -> Worker thread %ld finished\n",
        getpid(), id);

    return NULL;
}

void start_thread_pool(void)
{
    pthread_t threads[THREADS];

    for (long i = 0; i < THREADS; i++)
        pthread_create(&threads[i], NULL, worker, (void*)i);

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);
}

int main(void)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child process PID = %d\n", getpid());

        start_thread_pool();

        return 0;
    }

    printf("Parent process PID = %d\n", getpid());

    waitpid(pid, NULL, 0);

    printf("Child process completed\n");

    return 0;
}



