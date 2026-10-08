#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct message
{
    long type;
    char text[100];
};

int main()
{
    int msgid;

    msgid = msgget(
        IPC_PRIVATE,
        0666 | IPC_CREAT);

    if (msgid < 0)
    {
        perror("msgget");
        return 1;
    }

    printf(
        "Message queue created\n");

    printf(
        "Message Queue ID: %d\n",
        msgid);

    printf(
        "\nRun in another terminal:\n");

    printf(
        "ipcs -q\n");

    printf(
        "\nPress Enter to remove queue...\n");

    getchar();

    if (msgctl(
        msgid,
        IPC_RMID,
        NULL) < 0)
    {
        perror("msgctl");
        return 1;
    }

    printf(
        "Message queue removed\n");

    return 0;
}



