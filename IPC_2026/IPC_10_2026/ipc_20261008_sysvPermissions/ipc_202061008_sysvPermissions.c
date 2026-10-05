#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(void)
{
    int msgId;
    struct msqid_ds queueInfo;

    msgId = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

    if (msgId == -1)
    {
        perror("msgget");
        return EXIT_FAILURE;
    }

    /* --------------------------------------------------- */
    /* Get current permissions                             */
    /* --------------------------------------------------- */

    if (msgctl(msgId, IPC_STAT, &queueInfo) == -1)
    {
        perror("msgctl IPC_STAT");
        msgctl(msgId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("Current permissions: %o\n",
        queueInfo.msg_perm.mode & 0777);

    /* --------------------------------------------------- */
    /* Change permissions                                  */
    /* --------------------------------------------------- */

    queueInfo.msg_perm.mode =
        (queueInfo.msg_perm.mode & ~0777) | 0600;

    if (msgctl(msgId, IPC_SET, &queueInfo) == -1)
    {
        perror("msgctl IPC_SET");
        msgctl(msgId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    /* --------------------------------------------------- */
    /* Read permissions again                              */
    /* --------------------------------------------------- */

    if (msgctl(msgId, IPC_STAT, &queueInfo) == -1)
    {
        perror("msgctl IPC_STAT");
        msgctl(msgId, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("New permissions: %o\n",
        queueInfo.msg_perm.mode & 0777);

    msgctl(msgId, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}


