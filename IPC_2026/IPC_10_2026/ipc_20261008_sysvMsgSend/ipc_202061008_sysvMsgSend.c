#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct Message
{
    long messageType;
    char messageText[100];
};

int main(void)
{
    int msgId;
    struct Message message;

    msgId = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

    if (msgId == -1)
    {
        perror("msgget");
        return EXIT_FAILURE;
    }

    message.messageType = 1;

    strcpy(message.messageText,
        "Hello from System V Message Queue");

    if (msgsnd(msgId,
        &message,
        sizeof(message.messageText),
        0) == -1)
    {
        perror("msgsnd");
        msgctl(msgId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("Message sent successfully.\n");
    printf("Message: %s\n", message.messageText);
    printf("Message Queue ID: %d\n", msgId);

    msgctl(msgId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


