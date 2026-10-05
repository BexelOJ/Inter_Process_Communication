#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(void)
{
    int msgId;

    msgId = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

    if (msgId == -1)
    {
        perror("msgget");
        return EXIT_FAILURE;
    }

    printf("System V Message Queue created.\n");
    printf("Message Queue ID: %d\n", msgId);

    if (msgctl(msgId, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        return EXIT_FAILURE;
    }

    printf("Message Queue removed.\n");

    return EXIT_SUCCESS;
}


