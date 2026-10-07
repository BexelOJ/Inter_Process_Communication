#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/wait.h>
#include <string.h>

#define QUEUE_NAME "/kernel_ipc_mq"

int main(void)
{
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 128;
    attr.mq_curmsgs = 0;

    mqd_t mq = mq_open(
        QUEUE_NAME,
        O_CREAT | O_RDWR,
        0600,
        &attr
    );

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        mq_close(mq);
        mq_unlink(QUEUE_NAME);
        return 1;
    }

    if (pid == 0)
    {
        char buffer[128];

        ssize_t bytes = mq_receive(
            mq,
            buffer,
            sizeof(buffer),
            NULL
        );

        if (bytes >= 0)
        {
            buffer[bytes] = '\0';

            printf("Child received: %s\n", buffer);
        }
        else
        {
            perror("mq_receive");
        }

        mq_close(mq);

        return 0;
    }

    const char* message =
        "Hello through kernel message queue";

    if (mq_send(
        mq,
        message,
        strlen(message),
        1
    ) < 0)
    {
        perror("mq_send");
    }

    waitpid(pid, NULL, 0);

    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}



