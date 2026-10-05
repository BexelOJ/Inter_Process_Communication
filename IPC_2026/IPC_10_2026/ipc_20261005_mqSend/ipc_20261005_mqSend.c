/*
 * ipc_20261005_mqSend.c
 *
 * Demonstrates:
 *     - mq_open()
 *     - mq_send()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

// ---------------------------------------------------
#define MQ_NAME "/mq_send"
#define MAX_MSG_SIZE 128

// ---------------------------------------------------
int main(void)
{
    mqd_t mq;

    const char *message = "Message from sender";

    // ---------------------------------------------------
    // Open / create queue
    // ---------------------------------------------------
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

    // ---------------------------------------------------
    // Send message
    // ---------------------------------------------------
    if (mq_send(
            mq,
            message,
            strlen(message) + 1,
            0) == -1)
    {
        perror("mq_send");
        mq_close(mq);
        return EXIT_FAILURE;
    }

    printf("Message sent: %s\n", message);

    mq_close(mq);

    return EXIT_SUCCESS;
}



