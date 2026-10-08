#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <liburing.h>

#define QUEUE_DEPTH 1

int main()
{
    struct io_uring ring;

    if (io_uring_queue_init(
        QUEUE_DEPTH,
        &ring,
        0) < 0)
    {
        perror("io_uring_queue_init");
        return 1;
    }

    int fd = open(
        "data.txt",
        O_RDONLY);

    if (fd < 0)
    {
        perror("open");
        io_uring_queue_exit(&ring);
        return 1;
    }

    char buffer[1024];

    memset(
        buffer,
        0,
        sizeof(buffer));

    struct io_uring_sqe* sqe =
        io_uring_get_sqe(&ring);

    io_uring_prep_read(
        sqe,
        fd,
        buffer,
        sizeof(buffer) - 1,
        0);

    io_uring_submit(&ring);

    struct io_uring_cqe* cqe;

    int ret =
        io_uring_wait_cqe(
            &ring,
            &cqe);

    if (ret < 0)
    {
        fprintf(
            stderr,
            "wait_cqe failed\n");

        close(fd);
        io_uring_queue_exit(&ring);

        return 1;
    }

    if (cqe->res >= 0)
    {
        buffer[cqe->res] = '\0';

        printf(
            "Read %d bytes:\n%s\n",
            cqe->res,
            buffer);
    }
    else
    {
        printf(
            "I/O failed: %d\n",
            cqe->res);
    }

    io_uring_cqe_seen(
        &ring,
        cqe);

    close(fd);

    io_uring_queue_exit(&ring);

    return 0;
}



