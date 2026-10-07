#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
    float cpu;
    float memory;
    float temperature;

} SensorData;

SensorData collect_data(void)
{
    SensorData data;

    data.cpu = 20.0f + (rand() % 60);
    data.memory = 30.0f + (rand() % 50);
    data.temperature = 30.0f + (rand() % 20);

    return data;
}

int main(void)
{
    srand(time(NULL));

    printf("Data Collector started\n");

    for (int i = 0; i < 10; i++)
    {
        SensorData data = collect_data();

        printf("Sample %d\n", i + 1);

        printf("CPU         : %.2f %%\n",
            data.cpu);

        printf("Memory      : %.2f %%\n",
            data.memory);

        printf("Temperature : %.2f C\n",
            data.temperature);

        printf("-------------------------\n");

        sleep(1);
    }

    return EXIT_SUCCESS;
}



