#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    const char* request = "request.txt";
    const char* response = "response.txt";

    printf("File IPC server started\n");

    while (1)
    {
        if (access(request, F_OK) == 0)
        {
            printf("Request received\n");

            FILE* fp = fopen(request, "r");

            if (fp == NULL)
            {
                perror("fopen request");
                sleep(1);
                continue;
            }

            char buffer[256];

            if (fgets(buffer, sizeof(buffer), fp))
            {
                printf("Request: %s", buffer);
            }

            fclose(fp);

            fp = fopen(response, "w");

            if (fp == NULL)
            {
                perror("fopen response");
                return 1;
            }

            fprintf(fp, "Server response: request processed\n");
            fclose(fp);

            remove(request);

            printf("Response generated\n");
        }

        sleep(1);
    }

    return 0;
}



