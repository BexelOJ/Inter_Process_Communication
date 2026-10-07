#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char path[128];

    snprintf(
        path,
        sizeof(path),
        "/proc/%d/status",
        getpid()
    );

    FILE* fp = fopen(path, "r");

    if (!fp)
    {
        perror("fopen");
        return 1;
    }

    char line[512];

    while (fgets(line, sizeof(line), fp))
    {
        if (
            line[0] == 'N' ||
            line[0] == 'P' ||
            line[0] == 'U' ||
            line[0] == 'G' ||
            line[0] == 'T'
            )
        {
            printf("%s", line);
        }
    }

    fclose(fp);

    return 0;
}



