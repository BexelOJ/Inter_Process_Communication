/*
 * ipc_20261001_daemonProcess.c
 *
 * Demonstrates:
 *     - fork()
 *     - setsid()
 *     - chdir()
 *     - umask()
 *     - closing standard file descriptors
 *     - daemon process creation
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// ---------------------------------------------------
int main(void)
{
    pid_t pid;

    printf("Parent: Starting daemon creation\n");

    // ---------------------------------------------------
    // Step 1: Create child process
    // ---------------------------------------------------
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Parent process exits
    // ---------------------------------------------------
    if (pid > 0)
    {
        printf("Parent: Child created, PID = %d\n", pid);
        return EXIT_SUCCESS;
    }

    // ---------------------------------------------------
    // Child becomes session leader
    // ---------------------------------------------------
    if (setsid() < 0)
    {
        perror("setsid");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Change working directory
    // ---------------------------------------------------
    if (chdir("/") < 0)
    {
        perror("chdir");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Set file creation mask
    // ---------------------------------------------------
    umask(0);

    // ---------------------------------------------------
    // Close standard file descriptors
    // ---------------------------------------------------
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // ---------------------------------------------------
    // Daemon loop
    // ---------------------------------------------------
    while (1)
    {
        /*
         * Daemon work goes here.
         */

        sleep(5);
    }

    return EXIT_SUCCESS;
}



