#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char buffer[256];

    printf("Raspberry Pi IPC Service\n");

    printf("-------------------------\n");

    FILE* fp =
        fopen("/proc/cpuinfo", "r");

    if (fp == NULL)
    {
        perror("cpuinfo");

        return EXIT_FAILURE;
    }

    while (fgets(buffer,
        sizeof(buffer),
        fp))
    {
        if (buffer[0] == '\n')
            continue;

        printf("%s", buffer);
    }

    fclose(fp);

    printf("\n");

    printf("System uptime:\n");

    fp = fopen("/proc/uptime", "r");

    if (fp != NULL)
    {
        fgets(buffer,
            sizeof(buffer),
            fp);

        printf("%s\n",
            buffer);

        fclose(fp);
    }

    return EXIT_SUCCESS;
}



