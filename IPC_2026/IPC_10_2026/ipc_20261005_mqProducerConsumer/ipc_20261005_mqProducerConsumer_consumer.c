/*
 * ipc_20261005_mqProducerConsumer_consumer.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>

// ---------------------------------------------------
#define MQ_NAME "/mq_producer_consumer"
#define MAX_MSG_SIZE 128

// ---------------------------------------------------
int main(void)
{
    mqd_t mq;
    char buffer[MAX_MSG_SIZE];

    mq = mq_open(
        MQ_NAME,
        O_RDONLY
    );

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 5; i++)
    {
        mq_receive(
            mq,
            buffer,
            MAX_MSG_SIZE,
            NULL
        );

        printf("Consumer: %s\n", buffer);
    }

    mq_close(mq);
    mq_unlink(MQ_NAME);

    return EXIT_SUCCESS;
}



