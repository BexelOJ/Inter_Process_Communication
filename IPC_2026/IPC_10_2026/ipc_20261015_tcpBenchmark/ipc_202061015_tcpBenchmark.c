#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>

#define PORT 9090
#define TOTAL_SIZE (100 * 1024 * 1024)
#define BUFFER_SIZE (64 * 1024)

static double elapsed(struct timespec* start,
    struct timespec* end)
{
    return (end->tv_sec - start->tv_sec) +
        (end->tv_nsec - start->tv_nsec) / 1e9;
}

int main(void)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        int server = socket(AF_INET, SOCK_STREAM, 0);

        if (server == -1)
        {
            perror("socket");
            exit(1);
        }

        int opt = 1;

        setsockopt(server,
            SOL_SOCKET,
            SO_REUSEADDR,
            &opt,
            sizeof(opt));

        struct sockaddr_in addr = { 0 };

        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr =
            htonl(INADDR_LOOPBACK);

        bind(server,
            (struct sockaddr*)&addr,
            sizeof(addr));

        listen(server, 1);

        int client = accept(server, NULL, NULL);

        char buffer[BUFFER_SIZE];

        size_t received = 0;

        while (received < TOTAL_SIZE)
        {
            ssize_t n = recv(client,
                buffer,
                sizeof(buffer),
                0);

            if (n <= 0)
                break;

            received += n;
        }

        close(client);
        close(server);

        exit(0);
    }

    sleep(1);

    int client = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = { 0 };

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr =
        htonl(INADDR_LOOPBACK);

    if (connect(client,
        (struct sockaddr*)&addr,
        sizeof(addr)) == -1)
    {
        perror("connect");
        return 1;
    }

    char* buffer = malloc(BUFFER_SIZE);

    if (buffer == NULL)
    {
        perror("malloc");
        return 1;
    }

    memset(buffer, 'X', BUFFER_SIZE);

    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    size_t sent = 0;

    while (sent < TOTAL_SIZE)
    {
        size_t remaining = TOTAL_SIZE - sent;

        size_t amount =
            remaining < BUFFER_SIZE ?
            remaining : BUFFER_SIZE;

        ssize_t n = send(client,
            buffer,
            amount,
            0);

        if (n <= 0)
            break;

        sent += n;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    waitpid(pid, NULL, 0);

    double seconds = elapsed(&start, &end);

    double mbps =
        ((double)sent /
            (1024.0 * 1024.0))
        / seconds;

    printf("TCP localhost Benchmark\n");
    printf("Transferred : %.2f MB\n",
        (double)sent / (1024 * 1024));
    printf("Time        : %.6f seconds\n", seconds);
    printf("Throughput  : %.2f MB/s\n", mbps);

    free(buffer);
    close(client);

    return 0;
}



