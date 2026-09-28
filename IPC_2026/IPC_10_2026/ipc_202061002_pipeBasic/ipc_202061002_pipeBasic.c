#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    int pipeFd[2];
    char buffer[100];

    if (pipe(pipeFd) == -1)
    {
        perror("pipe");
        return 1;
    }

    const char* message = "Hello through pipe";

    write(pipeFd[1], message, strlen(message) + 1);

    read(pipeFd[0], buffer, sizeof(buffer));

    printf("Received: %s\n", buffer);

    close(pipeFd[0]);
    close(pipeFd[1]);

    return 0;
}


/*
//---------------------------------------------------

The pipe descriptors are:

pipeFd[0] → read
pipeFd[1] → write

//---------------------------------------------------
*/


