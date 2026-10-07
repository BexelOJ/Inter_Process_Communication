#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int pipe1[2];
    int pipe2[2];

    pipe(pipe1);
    pipe(pipe2);

    pid_t p1 = fork();

    if (p1 == 0)
    {
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);

        int value = 10;

        printf("Process 1: generated %d\n", value);

        write(pipe1[1], &value, sizeof(value));

        close(pipe1[1]);

        exit(EXIT_SUCCESS);
    }

    pid_t p2 = fork();

    if (p2 == 0)
    {
        close(pipe1[1]);
        close(pipe2[0]);

        int value;

        read(pipe1[0], &value, sizeof(value));

        printf("Process 2: received %d\n", value);

        value *= 2;

        printf("Process 2: produced %d\n", value);

        write(pipe2[1], &value, sizeof(value));

        close(pipe1[0]);
        close(pipe2[1]);

        exit(EXIT_SUCCESS);
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[1]);

    int value;

    read(pipe2[0], &value, sizeof(value));

    printf("Process 3: received %d\n", value);

    value += 5;

    printf("Process 3: final result = %d\n", value);

    close(pipe2[0]);

    wait(NULL);
    wait(NULL);

    return EXIT_SUCCESS;
}



