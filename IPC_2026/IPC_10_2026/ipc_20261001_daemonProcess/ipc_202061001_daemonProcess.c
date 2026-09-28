#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
    pid_t pid;
    int fd;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid > 0)
    {
        printf("Parent exiting\n");
        return 0;
    }

    /* Create new session */
    if (setsid() < 0)
    {
        perror("setsid");
        return 1;
    }

    /* Change working directory */
    if (chdir("/") < 0)
    {
        perror("chdir");
        return 1;
    }

    /* Set file permissions */
    umask(0);

    /* Redirect standard file descriptors */
    fd = open("/dev/null", O_RDWR);

    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    if (fd > STDERR_FILENO)
    {
        close(fd);
    }

    while (1)
    {
        sleep(10);
    }

    return 0;
}


/*
//---------------------------------------------------
fork()
  ↓
parent exits
  ↓
setsid()
  ↓
new session
  ↓
chdir("/")
  ↓
umask(0)
  ↓
redirect stdin/stdout/stderr
  ↓
background process

//---------------------------------------------------
*/


