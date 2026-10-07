#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

int main(void)
{
    fd_set read_fds;

    printf("Event-driven process started\n");

    while (1)
    {
        FD_ZERO(&read_fds);

        FD_SET(STDIN_FILENO, &read_fds);

        printf("Waiting for event...\n");
        fflush(stdout);

        int result = select(STDIN_FILENO + 1,
            &read_fds,
            NULL,
            NULL,
            NULL);

        if (result < 0)
        {
            perror("select");
            return EXIT_FAILURE;
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            char buffer[128];

            fgets(buffer, sizeof(buffer), stdin);

            printf("Event received: %s", buffer);

            if (buffer[0] == 'q')
            {
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}



