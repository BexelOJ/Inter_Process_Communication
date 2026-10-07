#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/spi/spidev.h>

#define SPI_DEVICE "/dev/spidev0.0"

int main(void)
{
    int fd;

    fd = open(SPI_DEVICE,
        O_RDWR);

    if (fd < 0)
    {
        perror("SPI open");

        return EXIT_FAILURE;
    }

    unsigned char mode = SPI_MODE_0;

    if (ioctl(fd,
        SPI_IOC_WR_MODE,
        &mode) < 0)
    {
        perror("SPI mode");

        close(fd);

        return EXIT_FAILURE;
    }

    unsigned char bits = 8;

    if (ioctl(fd,
        SPI_IOC_WR_BITS_PER_WORD,
        &bits) < 0)
    {
        perror("SPI bits");

        close(fd);

        return EXIT_FAILURE;
    }

    unsigned int speed = 1000000;

    if (ioctl(fd,
        SPI_IOC_WR_MAX_SPEED_HZ,
        &speed) < 0)
    {
        perror("SPI speed");

        close(fd);

        return EXIT_FAILURE;
    }

    unsigned char tx[] =
    {
        0x01,
        0x02,
        0x03,
        0x04
    };

    unsigned char rx[sizeof(tx)] = { 0 };

    struct spi_ioc_transfer transfer =
    {
        .tx_buf =
            (unsigned long)tx,

        .rx_buf =
            (unsigned long)rx,

        .len =
            sizeof(tx),

        .speed_hz =
            speed,

        .bits_per_word =
            bits
    };

    if (ioctl(fd,
        SPI_IOC_MESSAGE(1),
        &transfer) < 0)
    {
        perror("SPI transfer");

        close(fd);

        return EXIT_FAILURE;
    }

    printf("SPI received:\n");

    for (size_t i = 0;
        i < sizeof(rx);
        i++)
    {
        printf("0x%02X ",
            rx[i]);
    }

    printf("\n");

    close(fd);

    return EXIT_SUCCESS;
}



