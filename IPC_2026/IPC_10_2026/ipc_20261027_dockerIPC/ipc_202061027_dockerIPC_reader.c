ii
#include <stdio.h>

int main()
{
    FILE* file =
        fopen("/shared/message.txt", "r");

    if (!file)
    {
        perror("fopen");
        return 1;
    }

    char buffer[256];

    while (fgets(
        buffer,
        sizeof(buffer),
        file))
    {
        printf(
            "Container B: %s",
            buffer);
    }

    fclose(file);

    return 0;
}



