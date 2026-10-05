/*
 * ipc_20261005_mqBasic.c
 *
 * Demonstrates:
 *     - mq_open()
 *     - mq_send()
 *     - mq_receive()
 *     - mq_close()
 *     - mq_unlink()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

// ---------------------------------------------------
#define MQ_NAME "/mq_basic"
#define MAX_MSG_SIZE 128

// ---------------------------------------------------
int main(void)
{
    mqd_t mq;
    char buffer[MAX_MSG_SIZE];
    const char *message = "Hello from POSIX Message Queue";

    // ---------------------------------------------------
    // Create message queue
    // ---------------------------------------------------
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

    printf("Message queue created\n");

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
        mq_unlink(MQ_NAME);
        return EXIT_FAILURE;
    }

    printf("Message sent: %s\n", message);

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
        mq_unlink(MQ_NAME);
        return EXIT_FAILURE;
    }

    printf("Message received: %s\n", buffer);

    // ---------------------------------------------------
    // Cleanup
    // ---------------------------------------------------
    mq_close(mq);
    mq_unlink(MQ_NAME);

    return EXIT_SUCCESS;
}





