/*
 * ipc_20261005_mqProducerConsumer_producer.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>

// ---------------------------------------------------
#define MQ_NAME "/mq_producer_consumer"

// ---------------------------------------------------
int main(void)
{
    mqd_t mq;

    mq = mq_open(
        MQ_NAME,
        O_CREAT | O_WRONLY,
        0666,
        NULL
    );

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    for (int i = 1; i <= 5; i++)
    {
        char message[128];

        snprintf(
            message,
            sizeof(message),
            "Data %d",
            i
        );

        mq_send(
            mq,
            message,
            strlen(message) + 1,
            0
        );

        printf("Producer: %s\n", message);

        sleep(1);
    }

    mq_close(mq);

    return EXIT_SUCCESS;
}



