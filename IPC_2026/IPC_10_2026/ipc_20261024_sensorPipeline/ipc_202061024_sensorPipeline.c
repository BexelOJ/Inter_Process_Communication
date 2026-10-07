#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
    float temperature;

} SensorData;

static SensorData sensor_read(void)
{
    SensorData data;

    data.temperature =
        20.0f + rand() % 20;

    return data;
}

static int filter_data(SensorData data)
{
    if (data.temperature > 35.0f)
        return 0;

    return 1;
}

static float process_data(SensorData data)
{
    return data.temperature * 1.8f + 32.0f;
}

int main(void)
{
    srand(getpid());

    printf("Sensor pipeline started\n");

    while (1)
    {
        SensorData data;

        data = sensor_read();

        printf("Sensor: %.2f C\n",
            data.temperature);

        if (!filter_data(data))
        {
            printf("Filter: rejected\n");

            sleep(1);

            continue;
        }

        float result =
            process_data(data);

        printf("Processor: %.2f F\n",
            result);

        printf("--------------------\n");

        sleep(1);
    }

    return EXIT_SUCCESS;
}



