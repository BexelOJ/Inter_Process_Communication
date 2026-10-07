#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int pipefd[2];

void* writer(void* arg)
{
    const char* message = "Hello from thread";

    write(pipefd[1], message, strlen(message) + 1);

    printf("Writer: message sent\n");

    return NULL;
}

void* reader(void* arg)
{
    char buffer[128];

    read(pipefd[0], buffer, sizeof(buffer));

    printf("Reader: received -> %s\n", buffer);

    return NULL;
}

int main(void)
{
    pthread_t writer_thread;
    pthread_t reader_thread;

    pipe(pipefd);

    pthread_create(&reader_thread, NULL, reader, NULL);
    pthread_create(&writer_thread, NULL, writer, NULL);

    pthread_join(writer_thread, NULL);
    pthread_join(reader_thread, NULL);

    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}



