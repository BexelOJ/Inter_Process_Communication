#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    ROUTE_CPU,
    ROUTE_MEMORY,
    ROUTE_DISK,
    ROUTE_UNKNOWN
} Route;

Route get_route(const char* message)
{
    if (strncmp(message, "CPU:", 4) == 0)
        return ROUTE_CPU;

    if (strncmp(message, "MEM:", 4) == 0)
        return ROUTE_MEMORY;

    if (strncmp(message, "DISK:", 5) == 0)
        return ROUTE_DISK;

    return ROUTE_UNKNOWN;
}

void route_message(const char* message)
{
    switch (get_route(message))
    {
    case ROUTE_CPU:
        printf("Routing to CPU handler: %s\n", message);
        break;

    case ROUTE_MEMORY:
        printf("Routing to memory handler: %s\n", message);
        break;

    case ROUTE_DISK:
        printf("Routing to disk handler: %s\n", message);
        break;

    default:
        printf("Unknown message: %s\n", message);
        break;
    }
}

int main(void)
{
    const char* messages[] =
    {
        "CPU:75",
        "MEM:62",
        "DISK:81",
        "NETWORK:45",
        "CPU:40"
    };

    int count = sizeof(messages) / sizeof(messages[0]);

    for (int i = 0; i < count; i++)
    {
        route_message(messages[i]);
    }

    return EXIT_SUCCESS;
}



