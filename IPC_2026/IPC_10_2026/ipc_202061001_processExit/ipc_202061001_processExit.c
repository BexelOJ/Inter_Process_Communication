#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    printf("Process started\n");
    printf("PID : %d\n", getpid());

    printf("Process is exiting...\n");

    exit(0);

    printf("This line will never execute\n");

    return 0;
}


//---------------------------------------------------


