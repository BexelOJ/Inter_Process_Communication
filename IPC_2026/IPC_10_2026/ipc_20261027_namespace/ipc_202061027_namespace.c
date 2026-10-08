#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE (1024 * 1024)

int child_function(void* arg)
{
    printf(
        "Inside namespace\n");

    printf(
        "PID inside namespace: %d\n",
        getpid());

    printf(
        "Parent PID inside namespace: %d\n",
        getppid());

    sleep(5);

    return 0;
}

int main()
{
    char* stack =
        malloc(STACK_SIZE);

    if (!stack)
    {
        perror("malloc");
        return 1;
    }

    char* stack_top =
        stack + STACK_SIZE;

    pid_t pid = clone(
        child_function,
        stack_top,
        CLONE_NEWPID | SIGCHLD,
        NULL);

    if (pid < 0)
    {
        perror("clone");
        free(stack);
        return 1;
    }

    printf(
        "Parent PID: %d\n",
        getpid());

    printf(
        "Namespace child PID: %d\n",
        pid);

    waitpid(
        pid,
        NULL,
        0);

    free(stack);

    return 0;
}



