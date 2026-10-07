#include <stdio.h>
#include <unistd.h>

void check_permission(const char* file, int mode, const char* name)
{
    if (access(file, mode) == 0)
        printf("%s : YES\n", name);
    else
        printf("%s : NO\n", name);
}

int main(void)
{
    const char* file = "ipc_permission_test.txt";

    FILE* fp = fopen(file, "w");

    if (!fp)
    {
        perror("fopen");
        return 1;
    }

    fprintf(fp, "IPC permission test\n");

    fclose(fp);

    printf("File: %s\n\n", file);

    check_permission(file, R_OK, "Read");
    check_permission(file, W_OK, "Write");
    check_permission(file, X_OK, "Execute");

    return 0;
}



