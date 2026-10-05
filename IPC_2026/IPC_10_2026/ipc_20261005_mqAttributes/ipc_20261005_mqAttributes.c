/*
 * ipc_20261005_mqAttributes.c
 *
 * Demonstrates:
 *     - struct mq_attr
 *     - mq_getattr()
 *     - mq_setattr()
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

// ---------------------------------------------------
#define MQ_NAME "/mq_attributes"

// ---------------------------------------------------
int main(void)
{
    mqd_t mq;
    struct mq_attr attr;
    struct mq_attr new_attr;

    // ---------------------------------------------------
    // Configure queue attributes
    // ---------------------------------------------------
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 128;
    attr.mq_curmsgs = 0;

    // ---------------------------------------------------
    // Create queue
    // ---------------------------------------------------
    mq = mq_open(
        MQ_NAME,
        O_CREAT | O_RDWR,
        0666,
        &attr
    );

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Get attributes
    // ---------------------------------------------------
    if (mq_getattr(mq, &new_attr) == -1)
    {
        perror("mq_getattr");
        mq_close(mq);
        mq_unlink(MQ_NAME);
        return EXIT_FAILURE;
    }

    printf("Queue attributes:\n");
    printf("Flags      : %ld\n", new_attr.mq_flags);
    printf("Max msgs   : %ld\n", new_attr.mq_maxmsg);
    printf("Msg size   : %ld\n", new_attr.mq_msgsize);
    printf("Current    : %ld\n", new_attr.mq_curmsgs);

    mq_close(mq);
    mq_unlink(MQ_NAME);

    return EXIT_SUCCESS;
}



