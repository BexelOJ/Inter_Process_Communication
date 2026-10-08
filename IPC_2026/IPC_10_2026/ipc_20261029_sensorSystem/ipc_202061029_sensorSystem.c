// ---------------------------------------------------
// ipc_20261029_sensorSystem
// Sensor -> processor -> display
// Two Linux pipes
// ---------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

typedef struct
{
    int temperature;
    int humidity;
} SensorData;

int main(void)
{
    int sensor_to_processor[2];
    int processor_to_display[2];

    pipe(sensor_to_processor);
    pipe(processor_to_display);

    pid_t sensor_pid = fork();

    if (sensor_pid == 0)
    {
        close(sensor_to_processor[0]);

        srand(time(NULL) ^ getpid());

        for (int i = 0; i < 5; i++)
        {
            SensorData data;

            data.temperature =
                20 + rand() % 30;

            data.humidity =
                40 + rand() % 50;

            write(sensor_to_processor[1],
                &data,
                sizeof(data));

            sleep(1);
        }

        close(sensor_to_processor[1]);

        exit(0);
    }

    pid_t processor_pid = fork();

    if (processor_pid == 0)
    {
        close(sensor_to_processor[1]);
        close(processor_to_display[0]);

        SensorData data;

        while (read(sensor_to_processor[0],
            &data,
            sizeof(data)) > 0)
        {
            /*
             * Processing stage
             */
            data.temperature += 1;

            write(processor_to_display[1],
                &data,
                sizeof(data));
        }

        close(sensor_to_processor[0]);
        close(processor_to_display[1]);

        exit(0);
    }

    /* Parent = display */
    close(sensor_to_processor[0]);
    close(sensor_to_processor[1]);

    close(processor_to_display[1]);

    SensorData data;

    while (read(processor_to_display[0],
        &data,
        sizeof(data)) > 0)
    {
        printf("Sensor Result:\n");
        printf("Temperature: %d C\n",
            data.temperature);

        printf("Humidity: %d %%\n\n",
            data.humidity);
    }

    close(processor_to_display[0]);

    waitpid(sensor_pid, NULL, 0);
    waitpid(processor_pid, NULL, 0);

    printf("Sensor system stopped\n");

    return 0;
}



