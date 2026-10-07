#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <signal.h>
#include <poll.h>

#ifndef SYS_pidfd_open
#define SYS_pidfd_open 434
#endif

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
        printf("Child: PID = %d\n",
            getpid());

        sleep(3);

        printf("Child exiting...\n");

        _exit(42);
    }

    printf("Parent: child PID = %d\n",
        pid);

    /*
     * Obtain pidfd.
     */
    int pidfd = syscall(SYS_pidfd_open,
        pid,
        0);

    if (pidfd < 0)
    {
        perror("pidfd_open");
        return EXIT_FAILURE;
    }

    printf("Parent: pidfd = %d\n",
        pidfd);

    /*
     * Wait for process termination
     * using poll().
     */
    struct pollfd pfd;

    pfd.fd = pidfd;
    pfd.events = POLLIN;

    printf("Waiting for child through pidfd...\n");

    int result = poll(&pfd,
        1,
        -1);

    if (result < 0)
    {
        perror("poll");
        close(pidfd);
        return EXIT_FAILURE;
    }

    if (pfd.revents & POLLIN)
    {
        printf("Child process terminated.\n");
    }

    int status;

    waitpid(pid,
        &status,
        0);

    if (WIFEXITED(status))
    {
        printf("Exit status = %d\n",
            WEXITSTATUS(status));
    }

    close(pidfd);

    return EXIT_SUCCESS;
}



