/*
 * ipc_20261005_mqNonBlocking.c
 *
 * Demonstrates:
 *     - O_NONBLOCK
 *     - Non-blocking mq_receive()
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <errno.h>

// ---------------------------------------------------
#define MQ_NAME "/mq_nonblocking"
#define MAX_MSG_SIZE 128

// ---------------------------------------------------
int main(void)
{
    mqd_t mq;
    char buffer[MAX_MSG_SIZE];

    mq = mq_open(
        MQ_NAME,
        O_CREAT | O_RDONLY | O_NONBLOCK,
        0666,
        NULL
    );

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    printf("Trying to receive message...\n");

    if (mq_receive(
            mq,
            buffer,
            MAX_MSG_SIZE,
            NULL) == -1)
    {
        if (errno == EAGAIN)
        {
            printf("No message available\n");
        }
        else
        {
            perror("mq_receive");
        }
    }
    else
    {
        printf("Received: %s\n", buffer);
    }

    mq_close(mq);
    mq_unlink(MQ_NAME);

    return EXIT_SUCCESS;
}



