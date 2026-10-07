#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>

#define SOCKET_PATH "/tmp/ipc_secure.sock"

int main(void)
{
    int server_fd;

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("socket");
        return 1;
    }

    unlink(SOCKET_PATH);

    struct sockaddr_un address;

    address.sun_family = AF_UNIX;

    snprintf(
        address.sun_path,
        sizeof(address.sun_path),
        "%s",
        SOCKET_PATH
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
     * Owner: read/write
     * Group: read/write
     * Others: no access
     */
    chmod(SOCKET_PATH, 0660);

    struct stat st;

    stat(SOCKET_PATH, &st);

    printf("Unix socket: %s\n", SOCKET_PATH);

    printf("UID : %d\n", st.st_uid);
    printf("GID : %d\n", st.st_gid);

    printf("Mode: %o\n",
        st.st_mode & 0777);

    listen(server_fd, 5);

    printf("Server listening...\n");
    printf("Press Ctrl+C to exit\n");

    while (1)
        pause();

    close(server_fd);

    unlink(SOCKET_PATH);

    return 0;
}



