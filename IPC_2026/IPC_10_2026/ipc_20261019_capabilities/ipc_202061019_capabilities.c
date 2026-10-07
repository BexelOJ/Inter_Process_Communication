#include <stdio.h>
#include <string.h>
#include <unistd.h>

void show_capabilities(void)
{
    FILE* fp = fopen("/proc/self/status", "r");

    if (!fp)
    {
        perror("fopen");
        return;
    }

    char line[512];

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "CapInh:", 7) == 0 ||
            strncmp(line, "CapPrm:", 7) == 0 ||
            strncmp(line, "CapEff:", 7) == 0 ||
            strncmp(line, "CapBnd:", 7) == 0 ||
            strncmp(line, "CapAmb:", 7) == 0)
        {
            printf("%s", line);
        }
    }

    fclose(fp);
}

int main(void)
{
    printf("PID: %d\n\n", getpid());

    show_capabilities();

    return 0;
}



