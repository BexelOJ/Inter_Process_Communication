/*
 * ipc_20261005_mqReceiv.c
 *
 * Demonstrates:
 *     - mq_open()
 *     - mq_receive()
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>

// ---------------------------------------------------
#define MQ_NAME "/mq_send"
#define MAX_MSG_SIZE 128

// ---------------------------------------------------
int main(void)
{
    mqd_t mq;
    char buffer[MAX_MSG_SIZE];

    // ---------------------------------------------------
    // Open queue
    // ---------------------------------------------------
    mq = mq_open(
        MQ_NAME,
        O_RDONLY
    );

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Receive message
    // ---------------------------------------------------
    if (mq_receive(
            mq,
            buffer,
            MAX_MSG_SIZE,
            NULL) == -1)
    {
        perror("mq_receive");
        mq_close(mq);
        return EXIT_FAILURE;
    }

    printf("Message received: %s\n", buffer);

    mq_close(mq);

    return EXIT_SUCCESS;
}



