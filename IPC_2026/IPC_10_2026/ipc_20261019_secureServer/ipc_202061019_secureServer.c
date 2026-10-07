#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>

#define SOCKET_PATH "/tmp/secure_ipc_server.sock"

int main(void)
{
    int server_fd;
    int client_fd;

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("socket");
        return 1;
    }

    unlink(SOCKET_PATH);

    struct sockaddr_un address;

    memset(&address, 0, sizeof(address));

    address.sun_family = AF_UNIX;

    strncpy(
        address.sun_path,
        SOCKET_PATH,
        sizeof(address.sun_path) - 1
    );

    if (bind(
        server_fd,
        (struct sockaddr*)&address,
        sizeof(address)) < 0)
    {
        perror("bind");
        close(server_fd);
        return 1;
    }

    /*
     * Only owner and group can access socket.
     */
    chmod(SOCKET_PATH, 0660);

    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        close(server_fd);
        unlink(SOCKET_PATH);
        return 1;
    }

    printf("Secure IPC server listening\n");
    printf("Socket: %s\n", SOCKET_PATH);

    client_fd = accept(server_fd, NULL, NULL);

    if (client_fd < 0)
    {
        perror("accept");
        close(server_fd);
        unlink(SOCKET_PATH);
        return 1;
    }

    /*
     * Get peer credentials.
     */
    struct ucred peer;
    socklen_t peer_len = sizeof(peer);

    if (getsockopt(
        client_fd,
        SOL_SOCKET,
        SO_PEERCRED,
        &peer,
        &peer_len) < 0)
    {
        perror("getsockopt");
        close(client_fd);
        close(server_fd);
        unlink(SOCKET_PATH);
        return 1;
    }

    printf("\nPeer information\n");
    printf("----------------------------\n");
    printf("PID : %d\n", peer.pid);
    printf("UID : %d\n", peer.uid);
    printf("GID : %d\n", peer.gid);

    /*
     * Example policy:
     * accept only same UID as server.
     */
    if (peer.uid != getuid())
    {
        printf("ACCESS DENIED\n");

        close(client_fd);
        close(server_fd);
        unlink(SOCKET_PATH);

        return 1;
    }

    printf("ACCESS GRANTED\n");

    const char* response =
        "Authenticated IPC connection\n";

    write(
        client_fd,
        response,
        strlen(response)
    );

    close(client_fd);
    close(server_fd);

    unlink(SOCKET_PATH);

    return 0;
}



