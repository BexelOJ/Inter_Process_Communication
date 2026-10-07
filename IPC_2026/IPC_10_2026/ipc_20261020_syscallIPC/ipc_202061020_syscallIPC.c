#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(void)
{
    const char* message =
        "Hello directly through syscall()\n";

    long result = syscall(
        SYS_write,
        STDOUT_FILENO,
        message,
        35
    );

    if (result < 0)
    {
        perror("syscall");
        return 1;
    }

    printf(
        "syscall() returned %ld\n",
        result
    );

    return 0;
}



