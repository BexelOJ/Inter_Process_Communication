#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    const char* filename;
} FileRequest;

void* read_file(void* arg)
{
    FileRequest* request = (FileRequest*)arg;

    FILE* file = fopen(request->filename, "r");

    if (!file)
    {
        perror("fopen");
        return NULL;
    }

    char buffer[256];

    printf("Worker: reading file...\n");

    while (fgets(buffer, sizeof(buffer), file))
    {
        printf("FILE: %s", buffer);
    }

    fclose(file);

    printf("\nWorker: file operation completed\n");

    return NULL;
}

int main()
{
    pthread_t thread;

    FileRequest request =
    {
        .filename = "data.txt"
    };

    printf("Main: submitting file request\n");

    if (pthread_create(
        &thread,
        NULL,
        read_file,
        &request) != 0)
    {
        perror("pthread_create");
        return 1;
    }

    printf("Main: doing other work...\n");

    for (int i = 0; i < 5; i++)
    {
        printf("Main: work %d\n", i);
    }

    pthread_join(thread, NULL);

    printf("Main: finished\n");

    return 0;
}



