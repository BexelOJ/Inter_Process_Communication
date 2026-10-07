#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char name[32];
    char value[64];

} Config;

#define CONFIG_COUNT 4

static Config configs[CONFIG_COUNT] =
{
    {"DEVICE_NAME", "YantraSthiti"},
    {"LOG_LEVEL",    "INFO"},
    {"SERVER_IP",    "192.168.0.130"},
    {"PORT",         "8080"}
};

void get_config(const char* name)
{
    for (int i = 0; i < CONFIG_COUNT; i++)
    {
        if (strcmp(configs[i].name, name) == 0)
        {
            printf("%s = %s\n",
                configs[i].name,
                configs[i].value);

            return;
        }
    }

    printf("Configuration not found\n");
}

void set_config(const char* name,
    const char* value)
{
    for (int i = 0; i < CONFIG_COUNT; i++)
    {
        if (strcmp(configs[i].name, name) == 0)
        {
            strncpy(configs[i].value,
                value,
                sizeof(configs[i].value) - 1);

            printf("Configuration updated\n");

            return;
        }
    }

    printf("Configuration not found\n");
}

void show_config(void)
{
    for (int i = 0; i < CONFIG_COUNT; i++)
    {
        printf("%s = %s\n",
            configs[i].name,
            configs[i].value);
    }
}

int main(void)
{
    printf("Configuration Service\n\n");

    show_config();

    printf("\n");

    get_config("SERVER_IP");

    set_config("LOG_LEVEL", "DEBUG");

    printf("\nUpdated configuration:\n");

    show_config();

    return EXIT_SUCCESS;
}



