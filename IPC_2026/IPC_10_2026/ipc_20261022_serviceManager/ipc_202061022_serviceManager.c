#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void service(const char* name)
{
    printf("%s started (PID=%d)\n",
        name,
        getpid());

    for (int i = 1; i <= 3; i++)
    {
        printf("%s working: %d\n",
            name,
            i);

        sleep(1);
    }

    printf("%s stopped\n", name);
}

int main(void)
{
    pid_t services[3];

    const char* names[] =
    {
        "NetworkService",
        "StorageService",
        "LoggingService"
    };

    for (int i = 0; i < 3; i++)
    {
        services[i] = fork();

        if (services[i] == 0)
        {
            service(names[i]);

            exit(EXIT_SUCCESS);
        }
    }

    printf("Service manager started\n");

    for (int i = 0; i < 3; i++)
    {
        waitpid(services[i], NULL, 0);
    }

    printf("All services stopped\n");

    return EXIT_SUCCESS;
}



