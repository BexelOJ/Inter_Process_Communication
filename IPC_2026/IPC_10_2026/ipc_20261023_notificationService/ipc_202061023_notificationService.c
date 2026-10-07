#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/wait.h>

#define QUEUE_NAME "/notification_service"

int main(void)
{
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 128;
    attr.mq_curmsgs = 0;

    mqd_t mq =
        mq_open(QUEUE_NAME,
            O_CREAT | O_RDWR,
            0666,
            &attr);

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid == 0)
    {
        char message[128];

        for (int i = 0; i < 3; i++)
        {
            ssize_t size =
                mq_receive(mq,
                    message,
                    sizeof(message),
                    NULL);

            if (size >= 0)
            {
                message[size] = '\0';

                printf("[NOTIFICATION] %s\n",
                    message);
            }
        }

        mq_close(mq);

        exit(EXIT_SUCCESS);
    }

    const char* notifications[] =
    {
        "System started",
        "Temperature high",
        "System shutdown"
    };

    for (int i = 0; i < 3; i++)
    {
        mq_send(mq,
            notifications[i],
            strlen(notifications[i]),
            0);

        sleep(1);
    }

    wait(NULL);

    mq_close(mq);

    mq_unlink(QUEUE_NAME);

    return EXIT_SUCCESS;
}



