#include <stdio.h>

int main()
{
    FILE* file =
        fopen("/shared/message.txt", "w");

    if (!file)
    {
        perror("fopen");
        return 1;
    }

    fprintf(
        file,
        "Hello from Docker container A\n");

    fclose(file);

    printf("Message written\n");

    return 0;
}



