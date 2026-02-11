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
#define PORT 4567   // same fixed port number

int main()
{
    int sockfd;
    socklen_t addrlen;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE];
    ssize_t n;

    /* 1. Create UDP socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        exit(1);
    }

    /* 2. Initialize server address */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    addrlen = sizeof(serveraddr);

    /* 3. Fork process */
    if (fork() == 0)
    {
        /* CHILD: send messages */
        while (1)
        {
            printf("Client: ");
            scanf("%1023s", buff);

            sendto(sockfd, buff, strlen(buff) + 1, 0,
                   (struct sockaddr *)&serveraddr, addrlen);

            if (strcmp(buff, "BYE") == 0)
            {
                kill(getppid(), SIGKILL);
                break;
            }
        }
    }
    else
    {
        /* PARENT: receive messages */
        while (1)
        {
            n = recvfrom(sockfd, buff, sizeof(buff) - 1, 0,
                         NULL, NULL);
            if (n <= 0)
                continue;

            buff[n] = '\0';
            printf("\nServer: %s\n", buff);

            if (strcmp(buff, "BYE") == 0)
                break;
        }
    }

    close(sockfd);
    return 0;
}
