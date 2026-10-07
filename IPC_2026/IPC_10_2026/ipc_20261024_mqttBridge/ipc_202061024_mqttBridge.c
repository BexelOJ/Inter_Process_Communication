#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mosquitto.h>

#define MQTT_HOST "localhost"
#define MQTT_PORT 1883

#define MQTT_TOPIC "yantraSthiti/metrics"

int main(void)
{
    struct mosquitto* mosq;

    mosquitto_lib_init();

    mosq = mosquitto_new("ipc_mqtt_bridge",
        true,
        NULL);

    if (mosq == NULL)
    {
        fprintf(stderr,
            "mosquitto_new failed\n");

        return EXIT_FAILURE;
    }

    if (mosquitto_connect(mosq,
        MQTT_HOST,
        MQTT_PORT,
        60) != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr,
            "MQTT connection failed\n");

        mosquitto_destroy(mosq);

        mosquitto_lib_cleanup();

        return EXIT_FAILURE;
    }

    printf("MQTT bridge connected\n");

    const char* message =
        "{"
        "\"device\":\"Pi4\","
        "\"cpu\":42.5,"
        "\"ram\":61.2"
        "}";

    mosquitto_publish(mosq,
        NULL,
        MQTT_TOPIC,
        strlen(message),
        message,
        0,
        false);

    printf("Published:\n%s\n",
        message);

    mosquitto_loop(mosq,
        1000,
        1);

    mosquitto_disconnect(mosq);

    mosquitto_destroy(mosq);

    mosquitto_lib_cleanup();

    return EXIT_SUCCESS;
}



