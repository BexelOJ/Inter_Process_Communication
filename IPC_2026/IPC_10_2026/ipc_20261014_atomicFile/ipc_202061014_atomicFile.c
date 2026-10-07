#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    const char* tempFile = "data.tmp";
    const char* targetFile = "data.txt";

    FILE* fp = fopen(tempFile, "w");

    if (fp == NULL)
    {
        perror("fopen");
        return 1;
    }

    fprintf(fp, "New configuration data\n");
    fclose(fp);

    /*
     * rename() is atomic when source and destination
     * are on the same filesystem.
     */
    if (rename(tempFile, targetFile) == -1)
    {
        perror("rename");
        return 1;
    }

    printf("File updated atomically\n");

    return 0;
}



