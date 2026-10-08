#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 5000
#define MAX_EVENTS 10

int set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1)
        return -1;

    return fcntl(
        fd,
        F_SETFL,
        flags | O_NONBLOCK);
}

int main()
{
    int server_fd;
    int epoll_fd;

    struct sockaddr_in address;

    server_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0);

    int opt = 1;

    setsockopt(
        server_fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(
        server_fd,
        (struct sockaddr*)&address,
        sizeof(address));

    listen(server_fd, 10);

    set_nonblocking(server_fd);

    epoll_fd = epoll_create1(0);

    struct epoll_event event;

    event.events = EPOLLIN;
    event.data.fd = server_fd;

    epoll_ctl(
        epoll_fd,
        EPOLL_CTL_ADD,
        server_fd,
        &event);

    printf(
        "Async socket server listening on %d\n",
        PORT);

    struct epoll_event events[MAX_EVENTS];

    while (1)
    {
        int count = epoll_wait(
            epoll_fd,
            events,
            MAX_EVENTS,
            -1);

        for (int i = 0; i < count; i++)
        {
            int fd = events[i].data.fd;

            if (fd == server_fd)
            {
                int client_fd =
                    accept(server_fd, NULL, NULL);

                if (client_fd >= 0)
                {
                    set_nonblocking(client_fd);

                    event.events = EPOLLIN;
                    event.data.fd = client_fd;

                    epoll_ctl(
                        epoll_fd,
                        EPOLL_CTL_ADD,
                        client_fd,
                        &event);

                    printf(
                        "Client connected\n");
                }
            }
            else
            {
                char buffer[1024];

                int bytes = read(
                    fd,
                    buffer,
                    sizeof(buffer) - 1);

                if (bytes <= 0)
                {
                    close(fd);

                    epoll_ctl(
                        epoll_fd,
                        EPOLL_CTL_DEL,
                        fd,
                        NULL);
                }
                else
                {
                    buffer[bytes] = '\0';

                    printf(
                        "Received: %s\n",
                        buffer);

                    write(
                        fd,
                        "ACK\n",
                        4);
                }
            }
        }
    }

    close(server_fd);

    return 0;
}



