#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd;
    char buffer[256];

    fd = open("/dev/ipc_userspace", O_RDWR);

    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    write(fd, "Hello Kernel Driver", 19);

    lseek(fd, 0, SEEK_SET);

    int n = read(fd, buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        buffer[n] = '\0';
        printf("Userspace received: %s\n", buffer);
    }

    close(fd);

    return 0;
}




