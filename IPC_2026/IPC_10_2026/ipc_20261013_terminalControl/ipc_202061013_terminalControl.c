#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

int main(void)
{
    printf("Process PID : %d\n",
        getpid());

    printf("Process PGID: %d\n",
        getpgrp());

    if (!isatty(STDIN_FILENO))
    {
        printf("stdin is not a terminal.\n");
        return EXIT_SUCCESS;
    }

    pid_t terminalPgid =
        tcgetpgrp(STDIN_FILENO);

    if (terminalPgid < 0)
    {
        perror("tcgetpgrp");
        return EXIT_FAILURE;
    }

    printf("Terminal foreground PGID: %d\n",
        terminalPgid);

    printf("\nPress Ctrl+C to send SIGINT.\n");
    printf("Press Ctrl+Z to send SIGTSTP.\n");

    while (1)
    {
        sleep(1);
    }

    return EXIT_SUCCESS;
}



