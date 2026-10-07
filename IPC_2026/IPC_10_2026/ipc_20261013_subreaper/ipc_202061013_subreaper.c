#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/wait.h>

int main(void)
{
    if (prctl(PR_SET_CHILD_SUBREAPER,
        1) < 0)
    {
        perror("prctl");
        return EXIT_FAILURE;
    }

    printf("Subreaper PID = %d\n",
        getpid());

    pid_t child = fork();

    if (child < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child == 0)
    {
        /*
         * Child creates grandchild.
         */
        pid_t grandchild = fork();

        if (grandchild < 0)
        {
            perror("fork");
            _exit(EXIT_FAILURE);
        }

        if (grandchild == 0)
        {
            printf("Grandchild PID=%d\n",
                getpid());

            printf("Grandchild parent PID=%d\n",
                getppid());

            sleep(3);

            printf("After parent exits:\n");

            printf("Grandchild parent PID=%d\n",
                getppid());

            _exit(EXIT_SUCCESS);
        }

        printf("Child PID=%d exiting...\n",
            getpid());

        _exit(EXIT_SUCCESS);
    }

    waitpid(child,
        NULL,
        0);

    sleep(1);

    /*
     * Reap the adopted grandchild.
     */
    int status;

    pid_t result;

    while ((result = waitpid(-1,
        &status,
        WNOHANG)) > 0)
    {
        printf("Reaped PID=%d\n",
            result);
    }

    sleep(3);

    while (waitpid(-1,
        &status,
        WNOHANG) > 0)
    {
        printf("Reaped descendant.\n");
    }

    return EXIT_SUCCESS;
}



