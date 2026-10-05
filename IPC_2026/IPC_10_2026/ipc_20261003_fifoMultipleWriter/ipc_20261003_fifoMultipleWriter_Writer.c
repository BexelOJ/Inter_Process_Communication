#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "/tmp/ipc_fifo_multi"

int main(int argc, char *argv[])
{
    int fd;
    char message[256];

    if (argc != 2)
    {
        printf("Usage: %s <writer-name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    fd = open(FIFO_NAME, O_WRONLY);

    if (fd == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    snprintf(
        message,
        sizeof(message),
        "Message from %s",
        argv[1]
    );

    write(fd, message, strlen(message) + 1);

    printf("%s sent: %s\n", argv[1], message);

    close(fd);

    return EXIT_SUCCESS;
}



