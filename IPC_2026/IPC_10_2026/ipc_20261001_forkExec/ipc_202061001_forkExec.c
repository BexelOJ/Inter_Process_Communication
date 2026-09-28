#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child process\n");
        printf("Child PID : %d\n", getpid());

        execl("/bin/ls",
            "ls",
            "-l",
            NULL);

        perror("execl");
        exit(1);
    }

    printf("Parent process\n");
    printf("Parent PID : %d\n", getpid());
    printf("Waiting for child...\n");

    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
    {
        printf("Child exited\n");
        printf("Exit status : %d\n", WEXITSTATUS(status));
    }

    return 0;
}


/*
//---------------------------------------------------
Parent
  |
  | fork()
  |
  +------------------+
  |                  |
Parent              Child
                     |
                     | exec()
                     v
                   /bin/ls

//---------------------------------------------------
*/


