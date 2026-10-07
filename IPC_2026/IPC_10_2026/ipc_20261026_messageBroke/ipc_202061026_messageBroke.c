#include <stdio.h>
#include <mosquitto.h>

int main()
{
    struct mosquitto* mosq;

    mosquitto_lib_init();

    mosq = mosquitto_new(
        NULL,
        true,
        NULL);

    if (!mosq)
    {
        return 1;
    }

    if (mosquitto_connect(
        mosq,
        "localhost",
        1883,
        60) != MOSQ_ERR_SUCCESS)
    {
        printf("MQTT connection failed\n");

        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();

        return 1;
    }

    const char* message =
        "{\"temperature\":27.5}";

    mosquitto_publish(
        mosq,
        NULL,
        "sensor/temperature",
        strlen(message),
        message,
        1,
        false);

    printf("Message published\n");

    mosquitto_disconnect(mosq);

    mosquitto_destroy(mosq);

    mosquitto_lib_cleanup();

    return 0;
}



