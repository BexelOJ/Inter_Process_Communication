#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define SPOOL_DIR "spool"

int main(void)
{
    mkdir(SPOOL_DIR, 0777);

    printf("Spool consumer started\n");

    while (1)
    {
        DIR* dir = opendir(SPOOL_DIR);

        if (dir == NULL)
        {
            perror("opendir");
            return 1;
        }

        struct dirent* entry;

        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            char path[512];

            snprintf(path,
                sizeof(path),
                SPOOL_DIR "/%s",
                entry->d_name);

            printf("Processing: %s\n", path);

            FILE* fp = fopen(path, "r");

            if (fp != NULL)
            {
                char buffer[256];

                while (fgets(buffer, sizeof(buffer), fp))
                {
                    printf("Data: %s", buffer);
                }

                fclose(fp);
            }

            remove(path);

            printf("Processed and removed: %s\n", path);
        }

        closedir(dir);

        sleep(2);
    }

    return 0;
}



