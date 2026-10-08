#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    printf("Linux IPC Architecture\n");
    printf("=======================\n\n");

    printf("User space IPC:\n");
    printf("  1. Pipe\n");
    printf("  2. FIFO\n");
    printf("  3. Unix Domain Socket\n");
    printf("  4. TCP/IP Socket\n");
    printf("  5. Message Queue\n");
    printf("  6. Shared Memory\n");
    printf("  7. Signal\n");
    printf("  8. io_uring\n\n");

    printf("Kernel mechanisms:\n");
    printf("  Process management\n");
    printf("  File descriptors\n");
    printf("  Virtual memory\n");
    printf("  Scheduler\n");
    printf("  Namespaces\n");
    printf("  cgroups\n\n");

    pid_t pid = fork();

    if (pid == 0)
    {
        printf(
            "Child process: PID=%d\n",
            getpid());

        return 0;
    }

    wait(NULL);

    printf(
        "Parent process: PID=%d\n",
        getpid());

    return 0;
}



