#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <liburing.h>

#define QUEUE_DEPTH 1

int main()
{
    int pipefd[2];

    if (pipe(pipefd) < 0)
    {
        perror("pipe");
        return 1;
    }

    struct io_uring ring;

    if (io_uring_queue_init(
        QUEUE_DEPTH,
        &ring,
        0) < 0)
    {
        perror("io_uring");
        return 1;
    }

    const char* message =
        "Hello through pipe";

    write(
        pipefd[1],
        message,
        strlen(message));

    char buffer[256];

    struct io_uring_sqe* sqe =
        io_uring_get_sqe(&ring);

    io_uring_prep_read(
        sqe,
        pipefd[0],
        buffer,
        sizeof(buffer) - 1,
        0);

    io_uring_submit(&ring);

    struct io_uring_cqe* cqe;

    io_uring_wait_cqe(
        &ring,
        &cqe);

    if (cqe->res > 0)
    {
        buffer[cqe->res] = '\0';

        printf(
            "Received: %s\n",
            buffer);
    }

    io_uring_cqe_seen(
        &ring,
        cqe);

    io_uring_queue_exit(&ring);

    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}



