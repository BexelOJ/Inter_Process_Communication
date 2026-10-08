#include <stdio.h>
#include <unistd.h>

void show_file(const char* filename)
{
    FILE* file = fopen(filename, "r");

    if (!file)
    {
        perror(filename);
        return;
    }

    char buffer[512];

    while (fgets(
        buffer,
        sizeof(buffer),
        file))
    {
        printf("%s", buffer);
    }

    fclose(file);
}

int main()
{
    char path[256];

    pid_t pid = getpid();

    printf(
        "Current PID: %d\n\n",
        pid);

    printf(
        "========== /proc/self/status ==========\n");

    show_file(
        "/proc/self/status");

    printf(
        "\n========== /proc/self/stat ==========\n");

    show_file(
        "/proc/self/stat");

    printf(
        "\n========== /proc/self/limits ==========\n");

    show_file(
        "/proc/self/limits");

    printf(
        "\n========== /proc/self/maps ==========\n");

    show_file(
        "/proc/self/maps");

    printf(
        "\n========== /proc/self/fd ==========\n");

    snprintf(
        path,
        sizeof(path),
        "ls -l /proc/%d/fd",
        pid);

    system(path);

    return 0;
}



