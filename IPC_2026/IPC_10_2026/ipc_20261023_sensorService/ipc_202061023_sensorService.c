#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct
{
    float temperature;
    float humidity;
    float pressure;

} SensorData;

SensorData read_sensor(void)
{
    SensorData data;

    data.temperature =
        20.0f + (rand() % 200) / 10.0f;

    data.humidity =
        40.0f + (rand() % 400) / 10.0f;

    data.pressure =
        990.0f + (rand() % 300) / 10.0f;

    return data;
}

int main(void)
{
    srand(time(NULL));

    printf("Sensor Service started\n");

    while (1)
    {
        SensorData data = read_sensor();

        printf("Temperature : %.2f C\n",
            data.temperature);

        printf("Humidity    : %.2f %%\n",
            data.humidity);

        printf("Pressure    : %.2f hPa\n",
            data.pressure);

        printf("-----------------------------\n");

        sleep(2);
    }

    return EXIT_SUCCESS;
}



