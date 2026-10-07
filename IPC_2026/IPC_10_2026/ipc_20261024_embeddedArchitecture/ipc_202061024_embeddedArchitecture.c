#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
    float temperature;
    float humidity;

} SensorData;

static SensorData sensor_read(void)
{
    SensorData data;

    data.temperature = 30.5f;
    data.humidity = 65.2f;

    return data;
}

static SensorData process_data(SensorData data)
{
    data.temperature += 0.5f;

    return data;
}

static void communication_send(SensorData data)
{
    printf("[COMM] Temperature = %.2f C\n",
        data.temperature);

    printf("[COMM] Humidity = %.2f %%\n",
        data.humidity);
}

static void logger_write(SensorData data)
{
    printf("[LOG] T=%.2f H=%.2f\n",
        data.temperature,
        data.humidity);
}

int main(void)
{
    printf("Embedded architecture started\n");

    while (1)
    {
        SensorData data;

        data = sensor_read();

        data = process_data(data);

        communication_send(data);

        logger_write(data);

        sleep(2);
    }

    return EXIT_SUCCESS;
}



