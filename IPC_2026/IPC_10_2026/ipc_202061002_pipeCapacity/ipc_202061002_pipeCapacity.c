#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    int pipeFd[2];

    if (pipe(pipeFd) == -1)
    {
        perror("pipe");
        return 1;
    }

    long totalBytes = 0;

    /*
     * No reader is consuming data.
     *
     * Eventually write() will block when
     * the pipe becomes full.
     */

    char buffer[4096];

    for (int i = 0; i < sizeof(buffer); i++)
    {
        buffer[i] = 'A';
    }

    printf("Writing to pipe...\n");
    printf("This program will block when the pipe becomes full.\n");

    while (1)
    {
        ssize_t bytesWritten;

        bytesWritten = write(pipeFd[1],
            buffer,
            sizeof(buffer));

        if (bytesWritten == -1)
        {
            perror("write");
            break;
        }

        totalBytes += bytesWritten;

        printf("Total written: %ld bytes\n", totalBytes);
    }

    close(pipeFd[0]);
    close(pipeFd[1]);

    return 0;
}


/*

write()
write()
write()
write()
  ...
  ↓
PIPE FULL
  ↓
write()
  ↓
BLOCKED

*/
