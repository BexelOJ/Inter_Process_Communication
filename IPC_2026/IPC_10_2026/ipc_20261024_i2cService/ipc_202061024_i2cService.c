#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

#define I2C_DEVICE "/dev/i2c-1"
#define I2C_ADDRESS 0x3C

int main(void)
{
    int fd;

    fd = open(I2C_DEVICE,
        O_RDWR);

    if (fd < 0)
    {
        perror("I2C open");
        return EXIT_FAILURE;
    }

    if (ioctl(fd,
        I2C_SLAVE,
        I2C_ADDRESS) < 0)
    {
        perror("I2C address");

        close(fd);

        return EXIT_FAILURE;
    }

    printf("I2C service connected\n");

    /*
     * Example command.
     * Actual commands depend on the I2C device.
     */

    unsigned char command = 0x00;

    if (write(fd,
        &command,
        1) != 1)
    {
        perror("I2C write");
    }

    unsigned char data;

    if (read(fd,
        &data,
        1) == 1)
    {
        printf("Received: 0x%02X\n",
            data);
    }

    close(fd);

    return EXIT_SUCCESS;
}



