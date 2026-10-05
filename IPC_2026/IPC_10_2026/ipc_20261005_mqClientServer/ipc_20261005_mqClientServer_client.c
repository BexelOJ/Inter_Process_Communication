/*
 * ipc_20261005_mqClientServer_client.c
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

    char request[] = "Hello Server";
    char response[MAX_MSG_SIZE];

    // ---------------------------------------------------
    // Open server queue
    // ---------------------------------------------------
    serverMq = mq_open(
        SERVER_QUEUE,
        O_WRONLY
    );

    if (serverMq == (mqd_t)-1)
    {
        perror("server mq_open");
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Create client response queue
    // ---------------------------------------------------
    clientMq = mq_open(
        CLIENT_QUEUE,
        O_CREAT | O_RDONLY,
        0666,
        NULL
    );

    if (clientMq == (mqd_t)-1)
    {
        perror("client mq_open");
        mq_close(serverMq);
        return EXIT_FAILURE;
    }

    // ---------------------------------------------------
    // Send request
    // ---------------------------------------------------
    mq_send(
        serverMq,
        request,
        strlen(request) + 1,
        0
    );

    printf("Client: Request sent\n");

    // ---------------------------------------------------
    // Wait for server response
    // ---------------------------------------------------
    mq_receive(
        clientMq,
        response,
        MAX_MSG_SIZE,
        NULL
    );

    printf("Client received: %s\n", response);

    mq_close(serverMq);
    mq_close(clientMq);

    mq_unlink(CLIENT_QUEUE);

    return EXIT_SUCCESS;
}



