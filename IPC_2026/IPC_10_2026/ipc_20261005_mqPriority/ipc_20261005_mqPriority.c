/*
 * ipc_20261005_mqPriority.c
 *
 * Demonstrates:
 *     - mq_send()
 *     - message priorities
 *     - mq_receive()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

// ---------------------------------------------------
#define MQ_NAME "/mq_priority"
#define MAX_MSG_SIZE 128

// ---------------------------------------------------
int main(void)
{
    mqd_t mq;
    char buffer[MAX_MSG_SIZE];
    unsigned int priority;
    const char *low = "Low priority";
    const char *high = "High priority";

    mq = mq_open(
        MQ_NAME,
        O_CREAT | O_RDWR,
        0666,
        NULL
    );

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Send low priority message
    // ---------------------------------------------------
    mq_send(
        mq,
        low,
        strlen(low) + 1,
        1
    );

    // ---------------------------------------------------
    // Send high priority message
    // ---------------------------------------------------
    mq_send(
        mq,
        high,
        strlen(high) + 1,
        10
    );

    // ---------------------------------------------------
    // Receive first message
    // ---------------------------------------------------
    mq_receive(
        mq,
        buffer,
        MAX_MSG_SIZE,
        &priority
    );

    printf(
        "Received: %s | Priority: %u\n",
        buffer,
        priority
    );

    // ---------------------------------------------------
    // Receive second message
    // ---------------------------------------------------
    mq_receive(
        mq,
        buffer,
        MAX_MSG_SIZE,
        &priority
    );

    printf(
        "Received: %s | Priority: %u\n",
        buffer,
        priority
    );

    mq_close(mq);
    mq_unlink(MQ_NAME);

    return EXIT_SUCCESS;
}



