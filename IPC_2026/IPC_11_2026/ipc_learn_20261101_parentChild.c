#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void) {

    printf("Main Program Process ID is %d \n", getpid());
    printf("Shell Process ID is %d \n", getppid());

    pid_t pid;
    pid = fork();

    if (pid < 0) {
        printf("Child Process creation failed\n");
        exit(1);
    }

    if (pid == 0) {
        printf("\nWe are inside Child Process\n");
        printf("Child Process ID is %d \n", getpid());
        printf("Parent Process ID is %d \n", getppid());
    }

    else if (pid > 0) {
        printf("\nWe are inside Parent Process, and ID is %d \n", getpid());
        printf("Child Process ID is %d \n", pid);
    }

    else {
        
    }


    return 0;
}






