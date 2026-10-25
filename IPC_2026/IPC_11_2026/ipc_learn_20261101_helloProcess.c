#include <stdio.h> 
#include <unistd.h>

int main() {
    pid_t pid;

    printf("Main Program Process ID is %d\n", getpid());
    printf("Shell Process ID is %d\n", getppid());

    return 0;
}

