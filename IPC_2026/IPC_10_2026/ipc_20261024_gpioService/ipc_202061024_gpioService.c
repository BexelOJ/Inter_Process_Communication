#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/gpio.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_LINE 17

int main(void)
{
    int fd;

    struct gpiohandle_request request;

    fd = open(GPIO_CHIP, O_RDONLY);

    if (fd < 0)
    {
        perror("gpiochip");
        return EXIT_FAILURE;
    }

    request.lineoffsets[0] = GPIO_LINE;

    request.flags =
        GPIOHANDLE_REQUEST_OUTPUT;

    request.lines = 1;

    request.default_values[0] = 0;

    snprintf(request.consumer_label,
        sizeof(request.consumer_label),
        "ipc_gpio_service");

    if (ioctl(fd,
        GPIO_GET_LINEHANDLE_IOCTL,
        &request) < 0)
    {
        perror("GPIO_GET_LINEHANDLE_IOCTL");

        close(fd);

        return EXIT_FAILURE;
    }

    struct gpiohandle_data data;

    for (int i = 0; i < 10; i++)
    {
        data.values[0] = i % 2;

        if (ioctl(request.fd,
            GPIOHANDLE_SET_LINE_VALUES_IOCTL,
            &data) < 0)
        {
            perror("GPIO write");
            break;
        }

        printf("GPIO = %d\n",
            data.values[0]);

        sleep(1);
    }

    close(request.fd);

    close(fd);

    return EXIT_SUCCESS;
}



