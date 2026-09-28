#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Before exec()\n");
    printf("PID : %d\n", getpid());

    execl("/bin/ls",
        "ls",
        "-l",
        NULL);

    perror("execl");

    return 1;
}


//---------------------------------------------------


