#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(void)
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        /*
         * Child becomes process-group leader.
         */
        if (setpgid(0, 0) < 0)
        {
            perror("setpgid");
            _exit(EXIT_FAILURE);
        }

        printf("Child PID  = %d\n",
            getpid());

        printf("Child PGID = %d\n",
            getpgrp());

        sleep(10);

        _exit(EXIT_SUCCESS);
    }

    sleep(1);

    printf("Parent PID  = %d\n",
        getpid());

    printf("Child PID   = %d\n",
        pid);

    printf("Child PGID  = %d\n",
        getpgid(pid));

    /*
     * Send SIGTERM to the
     * entire process group.
     *
     * Negative PID means PGID.
     */
    printf("Sending SIGTERM to process group...\n");

    kill(-pid, SIGTERM);

    waitpid(pid,
        NULL,
        0);

    printf("Child terminated.\n");

    return EXIT_SUCCESS;
}



