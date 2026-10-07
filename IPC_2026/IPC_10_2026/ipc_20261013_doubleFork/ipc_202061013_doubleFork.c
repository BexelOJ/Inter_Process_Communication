#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    pid_t pid;

    /*
     * First fork
     */
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid > 0)
    {
        return EXIT_SUCCESS;
    }

    /*
     * First child creates a new session.
     */
    if (setsid() < 0)
    {
        perror("setsid");
        return EXIT_FAILURE;
    }

    /*
     * Second fork
     */
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid > 0)
    {
        return EXIT_SUCCESS;
    }

    /*
     * Grandchild becomes daemon.
     */

    chdir("/");

    umask(0);

    int fd = open("/dev/null",
        O_RDWR);

    if (fd < 0)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    if (fd > STDERR_FILENO)
        close(fd);

    while (1)
    {
        sleep(10);
    }

    return EXIT_SUCCESS;
}



