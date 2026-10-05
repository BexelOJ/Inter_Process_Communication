/*
 * ipc_20261005_mqNotification.c
 *
 * Demonstrates:
 *     - mq_notify()
 *     - SIGEV_SIGNAL
 *     - signal handling
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>

// ---------------------------------------------------
#define MQ_NAME "/mq_notification"
#define MAX_MSG_SIZE 128

static mqd_t mq;

// ---------------------------------------------------
static void notificationHandler(int sig)
{
    char buffer[MAX_MSG_SIZE];

    printf("Notification received!\n");

    if (mq_receive(
            mq,
            buffer,
            MAX_MSG_SIZE,
            NULL) == -1)
    {
        perror("mq_receive");
        return;
    }

    printf("Message received: %s\n", buffer);

    // ---------------------------------------------------
    // Register notification again
    // ---------------------------------------------------
    struct sigevent event;

    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGUSR1;

    if (mq_notify(mq, &event) == -1)
    {
        perror("mq_notify");
    }
}

// ---------------------------------------------------
int main(void)
{
    struct sigevent event;

    // ---------------------------------------------------
    // Create queue
    // ---------------------------------------------------
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

    // ---------------------------------------------------
    // Install signal handler
    // ---------------------------------------------------
    signal(SIGUSR1, notificationHandler);

    // ---------------------------------------------------
    // Configure notification
    // ---------------------------------------------------
    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGUSR1;

    if (mq_notify(mq, &event) == -1)
    {
        perror("mq_notify");
        mq_close(mq);
        mq_unlink(MQ_NAME);
        return EXIT_FAILURE;
    }

    printf("Waiting for message notification...\n");

    // ---------------------------------------------------
    // Keep process alive
    // ---------------------------------------------------
    while (1)
    {
        pause();
    }

    return EXIT_SUCCESS;
}



