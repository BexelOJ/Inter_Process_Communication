#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int pipeFd[2];
    pid_t pid;

    if (pipe(pipeFd) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        /* Consumer */

        int value;

        close(pipeFd[1]);

        while (read(pipeFd[0], &value, sizeof(value)) > 0)
        {
            printf("Consumer received: %d\n", value);
        }

        close(pipeFd[0]);
    }
    else
    {
        /* Producer */

        close(pipeFd[0]);

        for (int i = 1; i <= 10; i++)
        {
            printf("Producer sending: %d\n", i);

            write(pipeFd[1], &i, sizeof(i));

            sleep(1);
        }

        close(pipeFd[1]);

        wait(NULL);
    }

    return 0;
}


/*
//---------------------------------------------------
Producer writes numbers, consumer reads them.

Concept:

Producer
   |
   | 1
   | 2
   | 3
   | ...
   v
[ PIPE ]
   |
   v
Consumer

This is a simple introduction to the 
producer-consumer pattern.

//---------------------------------------------------
*/


