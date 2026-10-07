#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd1 = open("data.txt", O_CREAT | O_RDWR, 0666);

    if (fd1 == -1)
    {
        perror("open");
        return 1;
    }

    printf("fd1 = %d\n", fd1);

    int fd2 = dup(fd1);

    if (fd2 == -1)
    {
        perror("dup");
        close(fd1);
        return 1;
    }

    printf("fd2 = %d\n", fd2);

    printf("\nFD architecture:\n");

    printf("Process FD table\n");
    printf("    fd1 = %d ─────┐\n", fd1);
    printf("                  │\n");
    printf("    fd2 = %d ─────┤\n", fd2);
    printf("                  ↓\n");
    printf("             Open File\n");
    printf("                  ↓\n");
    printf("             data.txt\n");

    write(fd1, "Hello\n", 6);

    lseek(fd2, 0, SEEK_SET);

    char buffer[64] = { 0 };

    read(fd2, buffer, sizeof(buffer) - 1);

    printf("\nRead using fd2: %s", buffer);

    close(fd1);
    close(fd2);

    return 0;
}



