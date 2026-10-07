#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/seccomp.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <sys/prctl.h>
#include <sys/syscall.h>

int main(void)
{
    printf("PID: %d\n", getpid());

    /*
     * Require no_new_privs before installing
     * an unprivileged seccomp filter.
     */
    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0)
    {
        perror("PR_SET_NO_NEW_PRIVS");
        return 1;
    }

    struct sock_filter filter[] =
    {
        /*
         * Load system call number.
         */
        BPF_STMT(
            BPF_LD | BPF_W | BPF_ABS,
            offsetof(struct seccomp_data, nr)
        ),

            /*
             * If syscall == execve,
             * return EPERM.
             */
            BPF_JUMP(
                BPF_JMP | BPF_JEQ | BPF_K,
                __NR_execve,
                0,
                1
            ),

            BPF_STMT(
                BPF_RET | BPF_K,
                SECCOMP_RET_ERRNO | EPERM
            ),

            /*
             * Allow everything else.
             */
            BPF_STMT(
                BPF_RET | BPF_K,
                SECCOMP_RET_ALLOW
            )
    };

    struct sock_fprog program =
    {
        .len = sizeof(filter) / sizeof(filter[0]),
        .filter = filter
    };

    if (prctl(
        PR_SET_SECCOMP,
        SECCOMP_MODE_FILTER,
        &program) < 0)
    {
        perror("PR_SET_SECCOMP");
        return 1;
    }

    printf("Seccomp filter installed\n");
    printf("execve() is now blocked\n");

    char* args[] = {
        "/bin/ls",
        NULL
    };

    execve("/bin/ls", args, NULL);

    perror("execve");

    return 0;
}



