#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <mosquitto.h>

void on_connect(
    struct mosquitto* mosq,
    void* userdata,
    int rc)
{
    if (rc == 0)
    {
        printf("Connected to MQTT broker\n");

        mosquitto_subscribe(
            mosq,
            NULL,
            "yantraSthiti/metrics",
            1);
    }
}

void on_message(
    struct mosquitto* mosq,
    void* userdata,
    const struct mosquitto_message* message)
{
    printf(
        "Topic: %s\nMessage: %s\n",
        message->topic,
        (char*)message->payload);
}

int main()
{
    struct mosquitto* mosq;

    mosquitto_lib_init();

    mosq = mosquitto_new(
        "mqtt_ip_client",
        true,
        NULL);

    mosquitto_connect_callback_set(
        mosq,
        on_connect);

    mosquitto_message_callback_set(
        mosq,
        on_message);

    if (mosquitto_connect(
        mosq,
        "192.168.0.130",
        1883,
        60) != MOSQ_ERR_SUCCESS)
    {
        printf("Connection failed\n");
        return 1;
    }

    mosquitto_loop_forever(
        mosq,
        -1,
        1);

    mosquitto_destroy(mosq);

    mosquitto_lib_cleanup();

    return 0;
}



