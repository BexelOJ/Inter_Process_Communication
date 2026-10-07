#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

struct message
{
    long type;
    char text[128];
};

int main(void)
{
    int msgid;

    msgid = msgget(IPC_PRIVATE,
        0666 | IPC_CREAT);

    if (msgid < 0)
    {
        perror("msgget");
        return EXIT_FAILURE;
    }

    pid_t subscriber1 = fork();

    if (subscriber1 == 0)
    {
        struct message msg;

        msgrcv(msgid,
            &msg,
            sizeof(msg.text),
            1,
            0);

        printf("Subscriber 1 received: %s\n",
            msg.text);

        exit(EXIT_SUCCESS);
    }

    pid_t subscriber2 = fork();

    if (subscriber2 == 0)
    {
        struct message msg;

        msgrcv(msgid,
            &msg,
            sizeof(msg.text),
            2,
            0);

        printf("Subscriber 2 received: %s\n",
            msg.text);

        exit(EXIT_SUCCESS);
    }

    sleep(1);

    struct message msg;

    msg.type = 1;
    strcpy(msg.text, "Temperature = 30 C");

    msgsnd(msgid,
        &msg,
        sizeof(msg.text),
        0);

    msg.type = 2;
    strcpy(msg.text, "Temperature = 30 C");

    msgsnd(msgid,
        &msg,
        sizeof(msg.text),
        0);

    wait(NULL);
    wait(NULL);

    msgctl(msgid, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}



