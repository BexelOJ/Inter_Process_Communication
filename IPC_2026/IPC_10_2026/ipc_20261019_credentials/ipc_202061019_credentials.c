#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void print_credentials(const char* name)
{
    printf("\n%s\n", name);
    printf("-----------------------------\n");

    printf("PID        : %d\n", getpid());
    printf("PPID       : %d\n", getppid());

    printf("UID        : %d\n", getuid());
    printf("EUID       : %d\n", geteuid());

    printf("GID        : %d\n", getgid());
    printf("EGID       : %d\n", getegid());
}

int main(void)
{
    print_credentials("Parent");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        print_credentials("Child");
        return 0;
    }

    waitpid(pid, NULL, 0);

    return 0;
}



