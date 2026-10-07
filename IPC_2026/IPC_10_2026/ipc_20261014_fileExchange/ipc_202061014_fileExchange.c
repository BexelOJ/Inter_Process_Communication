#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    const char* file = "message.txt";

    FILE* fp = fopen(file, "w");

    if (fp == NULL)
    {
        perror("fopen");
        return 1;
    }

    fprintf(fp, "Hello from Producer\n");
    fclose(fp);

    printf("Producer wrote message\n");

    sleep(2);

    fp = fopen(file, "r");

    if (fp == NULL)
    {
        perror("fopen");
        return 1;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), fp))
    {
        printf("Consumer received: %s", buffer);
    }

    fclose(fp);

    return 0;
}



