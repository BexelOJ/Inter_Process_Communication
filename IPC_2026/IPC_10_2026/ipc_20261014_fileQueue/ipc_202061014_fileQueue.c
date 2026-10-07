#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define QUEUE_DIR "queue"

int main(void)
{
    mkdir(QUEUE_DIR, 0777);

    printf("Enter messages. Type 'exit' to stop.\n");

    char message[256];
    int counter = 0;

    while (1)
    {
        printf("> ");

        if (fgets(message, sizeof(message), stdin) == NULL)
            break;

        message[strcspn(message, "\n")] = '\0';

        if (strcmp(message, "exit") == 0)
            break;

        char filename[256];

        snprintf(filename,
            sizeof(filename),
            QUEUE_DIR "/message_%06d.txt",
            counter++);

        FILE* fp = fopen(filename, "w");

        if (fp == NULL)
        {
            perror("fopen");
            continue;
        }

        fprintf(fp, "%s\n", message);
        fclose(fp);

        printf("Queued: %s\n", filename);
    }

    return 0;
}



