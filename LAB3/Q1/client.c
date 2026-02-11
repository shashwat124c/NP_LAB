#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define MAXSIZE 1024
#define PORT 5000   

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE];
    ssize_t n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    printf("Connected to server\n");

    /* 4. Fork process for full duplex communication */
    if (fork() == 0)
    {
        /* CHILD: Sends messages to server */
        printf("CLIENT CHILD PROCESS\n");
        printf("PID = %d, PPID = %d\n", getpid(), getppid());

        while (1)
        {
            printf("Client: ");
            scanf("%1023s", buff);

            send(sockfd, buff, strlen(buff) + 1, 0);

            if (strcmp(buff, "BYE") == 0)
            {
                kill(getppid(), SIGKILL);
                break;
            }
        }
    }
    else
    {
        /* PARENT: Receives messages from server */
        printf("CLIENT PARENT PROCESS\n");
        printf("PID = %d, PPID = %d\n", getpid(), getppid());

        while (1)
        {
            n = recv(sockfd, buff, sizeof(buff) - 1, 0);
            if (n <= 0)
                break;

            buff[n] = '\0';
            printf("\nServer: %s\n", buff);

            if (strcmp(buff, "BYE") == 0)
                break;
        }
    }

    close(sockfd);
    return 0;
}
