/*
 * ipc_20261005_mqClientServer_server.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

// ---------------------------------------------------
#define SERVER_QUEUE "/mq_server"
#define CLIENT_QUEUE "/mq_client"

#define MAX_MSG_SIZE 128

// ---------------------------------------------------
int main(void)
{
    mqd_t serverMq;
    mqd_t clientMq;

    char request[MAX_MSG_SIZE];
    char response[MAX_MSG_SIZE];

    // ---------------------------------------------------
    // Create server queue
    // ---------------------------------------------------
    serverMq = mq_open(
        SERVER_QUEUE,
        O_CREAT | O_RDONLY,
        0666,
        NULL
    );

    if (serverMq == (mqd_t)-1)
    {
        perror("server mq_open");
        return EXIT_FAILURE;
    }

    printf("Server: Waiting for client request...\n");

    // ---------------------------------------------------
    // Receive request
    // ---------------------------------------------------
    mq_receive(
        serverMq,
        request,
        MAX_MSG_SIZE,
        NULL
    );

    printf("Server received: %s\n", request);

    // ---------------------------------------------------
    // Open client queue
    // ---------------------------------------------------
    clientMq = mq_open(
        CLIENT_QUEUE,
        O_WRONLY
    );

    if (clientMq == (mqd_t)-1)
    {
        perror("client mq_open");
        mq_close(serverMq);
        mq_unlink(SERVER_QUEUE);
        return EXIT_FAILURE;
    }

    snprintf(
        response,
        sizeof(response),
        "Server response received"
    );

    // ---------------------------------------------------
    // Send response
    // ---------------------------------------------------
    mq_send(
        clientMq,
        response,
        strlen(response) + 1,
        0
    );

    printf("Server: Response sent\n");

    mq_close(clientMq);
    mq_close(serverMq);

    mq_unlink(SERVER_QUEUE);

    return EXIT_SUCCESS;
}



