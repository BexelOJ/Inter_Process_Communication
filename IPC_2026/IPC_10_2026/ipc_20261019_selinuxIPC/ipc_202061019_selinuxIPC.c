#include <stdio.h>
#include <unistd.h>

int main(void)
{
    FILE* fp;

    printf("SELinux status\n");
    printf("============================\n");

    fp = fopen("/sys/fs/selinux/enforce", "r");

    if (!fp)
    {
        printf("SELinux filesystem is not available\n");
    }
    else
    {
        int enforce;

        if (fscanf(fp, "%d", &enforce) == 1)
        {
            if (enforce)
                printf("SELinux: enforcing\n");
            else
                printf("SELinux: permissive\n");
        }

        fclose(fp);
    }

    printf("\nProcess security context:\n");

    fp = fopen("/proc/self/attr/current", "r");

    if (!fp)
    {
        printf("Security context unavailable\n");
        return 0;
    }

    char context[512];

    if (fgets(context, sizeof(context), fp))
        printf("%s", context);

    fclose(fp);

    return 0;
}



