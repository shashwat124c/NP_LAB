#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 5000
#define MAX 1024

int client_count = 0;

int main()
{
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char buffer[MAX];
    FILE *fp;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 5);

    printf("Concurrent Server running on port %d...\n", PORT);

    while (1)
    {
        len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
        client_count++;

        if (client_count > 2)
        {
            strcpy(buffer, "terminate session");
            send(connfd, buffer, strlen(buffer) + 1, 0);
            close(connfd);
            printf("More than 2 clients. Server terminating.\n");
            exit(0);
        }

        if (fork() == 0)
        {
            close(listenfd);

            recv(connfd, buffer, MAX, 0);

            /* Append client string to file */
            fp = fopen("base.txt", "a");
            fprintf(fp, " %s", buffer);
            fclose(fp);

            /* Read final string */
            fp = fopen("base.txt", "r");
            char final[MAX];
            fgets(final, MAX, fp);
            fclose(fp);

            printf("\nClient %d connected from %s:%d\n",
                   client_count,
                   inet_ntoa(cliaddr.sin_addr),
                   ntohs(cliaddr.sin_port));

            printf("Final string: %s\n", final);

            send(connfd, final, strlen(final) + 1, 0);
            close(connfd);
            exit(0);
        }

        close(connfd);
        waitpid(-1, NULL, WNOHANG);
    }
}
