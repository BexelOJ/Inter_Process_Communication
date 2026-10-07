#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char filename[] = "/tmp/ipc_temp_XXXXXX";

    int fd = mkstemp(filename);

    if (fd == -1)
    {
        perror("mkstemp");
        return 1;
    }

    printf("Temporary file: %s\n", filename);
    printf("File descriptor: %d\n", fd);

    const char* message = "Temporary IPC data\n";

    write(fd, message, 19);

    printf("Temporary file created\n");

    sleep(5);

    close(fd);

    unlink(filename);

    printf("Temporary file removed\n");

    return 0;
}



