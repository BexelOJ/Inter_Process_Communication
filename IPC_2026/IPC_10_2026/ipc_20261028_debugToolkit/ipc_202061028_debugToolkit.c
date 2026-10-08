#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("====================================\n");
    printf("        Linux Debug Toolkit\n");
    printf("====================================\n");

    printf("PID        : %d\n", getpid());
    printf("PPID       : %d\n", getppid());
    printf("UID        : %d\n", getuid());
    printf("GID        : %d\n", getgid());

    printf("\nUseful debugging commands:\n");

    printf("  strace -p <PID>\n");
    printf("  lsof -p <PID>\n");
    printf("  ss -lntp\n");
    printf("  cat /proc/<PID>/status\n");
    printf("  cat /proc/<PID>/maps\n");
    printf("  cat /proc/<PID>/fd\n");
    printf("  perf top -p <PID>\n");

    printf("\nProcess is sleeping...\n");

    sleep(30);

    return 0;
}



