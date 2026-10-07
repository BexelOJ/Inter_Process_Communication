#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>

int sockets[2];

void* sender(void* arg)
{
    const char* message = "Hello through Unix socket";

    write(sockets[0], message, strlen(message) + 1);

    printf("Sender: message sent\n");

    return NULL;
}

void* receiver(void* arg)
{
    char buffer[128];

    read(sockets[1], buffer, sizeof(buffer));

    printf("Receiver: %s\n", buffer);

    return NULL;
}

int main(void)
{
    pthread_t sender_thread;
    pthread_t receiver_thread;

    socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);

    pthread_create(&receiver_thread, NULL, receiver, NULL);
    pthread_create(&sender_thread, NULL, sender, NULL);

    pthread_join(sender_thread, NULL);
    pthread_join(receiver_thread, NULL);

    close(sockets[0]);
    close(sockets[1]);

    return 0;
}



