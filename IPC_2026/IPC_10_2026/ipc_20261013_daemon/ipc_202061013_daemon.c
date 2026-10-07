#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
    pid_t pid;

    /*
     * 1. Fork
     */
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid > 0)
    {
        /* Parent exits */
        return EXIT_SUCCESS;
    }

    /*
     * 2. Create new session
     */
    if (setsid() < 0)
    {
        perror("setsid");
        return EXIT_FAILURE;
    }

    /*
     * 3. Change working directory
     */
    if (chdir("/") < 0)
    {
        perror("chdir");
        return EXIT_FAILURE;
    }

    /*
     * 4. Set file permissions
     */
    umask(0);

    /*
     * 5. Redirect standard descriptors
     */
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

    /*
     * Daemon work
     */
    while (1)
    {
        sleep(5);
    }

    return EXIT_SUCCESS;
}



