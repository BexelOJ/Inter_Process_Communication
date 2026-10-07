#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <termios.h>

#define UART_DEVICE "/dev/serial0"

int main(void)
{
    int fd;

    fd = open(UART_DEVICE,
        O_RDWR |
        O_NOCTTY);

    if (fd < 0)
    {
        perror("UART open");

        return EXIT_FAILURE;
    }

    struct termios tty;

    if (tcgetattr(fd, &tty) != 0)
    {
        perror("tcgetattr");

        close(fd);

        return EXIT_FAILURE;
    }

    cfsetispeed(&tty,
        B115200);

    cfsetospeed(&tty,
        B115200);

    tty.c_cflag =
        (tty.c_cflag & ~CSIZE) |
        CS8;

    tty.c_cflag |=
        CLOCAL | CREAD;

    tty.c_cflag &=
        ~(PARENB | PARODD);

    tty.c_cflag &=
        ~CSTOPB;

    tty.c_cflag &=
        ~CRTSCTS;

    tty.c_lflag = 0;

    tty.c_oflag = 0;

    tty.c_iflag = 0;

    if (tcsetattr(fd,
        TCSANOW,
        &tty) != 0)
    {
        perror("tcsetattr");

        close(fd);

        return EXIT_FAILURE;
    }

    printf("UART service started\n");

    const char* message =
        "Hello from Raspberry Pi UART\n";

    write(fd,
        message,
        strlen(message));

    char buffer[128];

    while (1)
    {
        ssize_t count =
            read(fd,
                buffer,
                sizeof(buffer) - 1);

        if (count > 0)
        {
            buffer[count] = '\0';

            printf("UART RX: %s",
                buffer);
        }

        usleep(100000);
    }

    close(fd);

    return EXIT_SUCCESS;
}



