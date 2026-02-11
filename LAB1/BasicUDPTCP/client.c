#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXSIZE 50

int main()
{
    int sockfd;
    ssize_t sentbytes, recedbytes;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE];

    /* Create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        exit(1);
    }

    /* Server address */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Connect */
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Connect failed");
        close(sockfd);
        exit(1);
    }

    while (1)
    {
        memset(buff, 0, sizeof(buff)); //clears buff for us so that prev value if thrown
        printf("Client: ");
        scanf("%49s", buff);

        sentbytes = send(sockfd, buff, strlen(buff) + 1, 0);
        if (sentbytes == -1)
        {
            perror("Send failed");
            break;
        }

        if (strcmp(buff, "stop") == 0)
            break;

        memset(buff, 0, sizeof(buff));
        recedbytes = recv(sockfd, buff, sizeof(buff), 0);
        if (recedbytes <= 0)
        {
            printf("Server disconnected\n");
            break;
        }

        printf("Server: %s\n", buff);

        if (strcmp(buff, "stop") == 0)
            break;
    }

    close(sockfd);
    return 0;
}
