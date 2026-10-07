#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t childPid = fork();

    if (childPid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (childPid == 0)
    {
        printf("Child PID=%d\n",
            getpid());

        sleep(3);

        printf("Child exiting with status 42\n");

        _exit(42);
    }

    printf("Parent PID=%d\n",
        getpid());

    printf("Child PID=%d\n",
        childPid);

    /*
     * Non-blocking check.
     */
    int status;

    pid_t result;

    result = waitpid(childPid,
        &status,
        WNOHANG);

    if (result == 0)
    {
        printf("Child is still running.\n");
    }
    else if (result == childPid)
    {
        printf("Child already terminated.\n");
    }

    /*
     * Now block until child terminates.
     */
    printf("Parent waiting...\n");

    result = waitpid(childPid,
        &status,
        0);

    if (result < 0)
    {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status))
    {
        printf("Child exited normally.\n");

        printf("Exit status = %d\n",
            WEXITSTATUS(status));
    }

    if (WIFSIGNALED(status))
    {
        printf("Child terminated by signal %d\n",
            WTERMSIG(status));
    }

    if (WIFSTOPPED(status))
    {
        printf("Child stopped by signal %d\n",
            WSTOPSIG(status));
    }

    return EXIT_SUCCESS;
}



