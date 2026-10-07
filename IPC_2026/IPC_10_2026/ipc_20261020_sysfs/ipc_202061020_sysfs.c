#include <stdio.h>
#include <dirent.h>

int main(void)
{
    const char* path = "/sys/class";

    DIR* directory = opendir(path);

    if (!directory)
    {
        perror("opendir");
        return 1;
    }

    printf("Kernel sysfs classes:\n");
    printf("============================\n");

    struct dirent* entry;

    while ((entry = readdir(directory)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;

        printf("%s\n", entry->d_name);
    }

    closedir(directory);

    return 0;
}



