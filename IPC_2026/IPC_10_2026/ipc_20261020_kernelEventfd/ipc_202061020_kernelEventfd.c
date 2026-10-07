#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/eventfd.h>
#include <sys/wait.h>

int main(void)
{
    int efd = eventfd(0, 0);

    if (efd < 0)
    {
        perror("eventfd");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        sleep(1);

        uint64_t value = 5;

        printf("Child: sending event = %lu\n", value);

        write(efd, &value, sizeof(value));

        close(efd);

        return 0;
    }

    uint64_t value;

    printf("Parent: waiting for event...\n");

    read(
        efd,
        &value,
        sizeof(value)
    );

    printf("Parent: event received = %lu\n", value);

    waitpid(pid, NULL, 0);

    close(efd);

    return 0;
}



