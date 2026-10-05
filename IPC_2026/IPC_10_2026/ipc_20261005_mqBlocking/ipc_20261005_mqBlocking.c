/*
 * ipc_20261005_mqBlocking.c
 *
 * Demonstrates:
 *     Blocking mq_receive()
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>

// ---------------------------------------------------
#define MQ_NAME "/mq_blocking"
#define MAX_MSG_SIZE 128

// ---------------------------------------------------
int main(void)
{
    mqd_t mq;
    char buffer[MAX_MSG_SIZE];

    mq = mq_open(
        MQ_NAME,
        O_CREAT | O_RDONLY,
        0666,
        NULL
    );

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    printf("Receiver started\n");
    printf("Waiting for message...\n");

    // ---------------------------------------------------
    // Blocking receive
    // ---------------------------------------------------
    if (mq_receive(
            mq,
            buffer,
            MAX_MSG_SIZE,
            NULL) == -1)
    {
        perror("mq_receive");
        mq_close(mq);
        mq_unlink(MQ_NAME);
        return EXIT_FAILURE;
    }

    printf("Received: %s\n", buffer);

    mq_close(mq);
    mq_unlink(MQ_NAME);

    return EXIT_SUCCESS;
}



