#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        printf("Before setsid():\n");

        printf("PID  = %d\n",
            getpid());

        printf("PGID = %d\n",
            getpgrp());

        printf("SID  = %d\n",
            getsid(0));

        printf("\nCreating new session...\n");

        if (setsid() < 0)
        {
            perror("setsid");
            _exit(EXIT_FAILURE);
        }

        printf("\nAfter setsid():\n");

        printf("PID  = %d\n",
            getpid());

        printf("PGID = %d\n",
            getpgrp());

        printf("SID  = %d\n",
            getsid(0));

        _exit(EXIT_SUCCESS);
    }

    waitpid(pid,
        NULL,
        0);

    return EXIT_SUCCESS;
}



