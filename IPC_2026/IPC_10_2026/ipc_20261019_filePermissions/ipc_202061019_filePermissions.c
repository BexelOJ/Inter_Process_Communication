#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
    const char* filename = "ipc_secure_file.txt";

    int fd = open(
        filename,
        O_CREAT | O_WRONLY | O_TRUNC,
        0600
    );

    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    const char* message = "Private IPC data\n";

    write(fd, message, 18);

    close(fd);

    struct stat st;

    if (stat(filename, &st) < 0)
    {
        perror("stat");
        return 1;
    }

    printf("File: %s\n", filename);

    printf("UID : %d\n", st.st_uid);
    printf("GID : %d\n", st.st_gid);

    printf("Permissions: %o\n",
        st.st_mode & 0777);

    return 0;
}



