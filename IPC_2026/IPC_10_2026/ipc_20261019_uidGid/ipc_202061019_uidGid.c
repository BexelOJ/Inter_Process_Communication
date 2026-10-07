#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Process ID : %d\n", getpid());

    printf("\nReal UID      : %d\n", getuid());
    printf("Effective UID : %d\n", geteuid());

    printf("\nReal GID      : %d\n", getgid());
    printf("Effective GID : %d\n", getegid());

    return 0;
}


