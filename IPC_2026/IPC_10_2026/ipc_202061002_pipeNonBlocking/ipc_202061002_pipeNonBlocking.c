#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(void)
{
    int pipeFd[2];
    char buffer[100];
    ssize_t result;

    if (pipe(pipeFd) == -1)
    {
        perror("pipe");
        return 1;
    }

    int flags = fcntl(pipeFd[0], F_GETFL);

    if (flags == -1)
    {
        perror("fcntl");
        return 1;
    }

    if (fcntl(pipeFd[0], F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl");
        return 1;
    }

    printf("Reading from empty pipe...\n");

    result = read(pipeFd[0], buffer, sizeof(buffer));

    if (result == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            printf("No data available - read() did not block\n");
        }
        else
        {
            perror("read");
        }
    }
    else
    {
        printf("Read %zd bytes\n", result);
    }

    close(pipeFd[0]);
    close(pipeFd[1]);

    return 0;
}


/*
//---------------------------------------------------
Uses O_NONBLOCK

Difference:

Blocking pipe:

read()
 ↓
wait for data


Non-blocking pipe:

read()
 ↓
EAGAIN / EWOULDBLOCK

//---------------------------------------------------
*/



