#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXSIZE 90

int main()
{
    int sockfd, newsockfd;
    ssize_t sentbytes, recedbytes;
    socklen_t addrlen;
    struct sockaddr_in serveraddr, clientaddr;
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
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bind */
    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    /* Listen */
    if (listen(sockfd, 1) == -1)
    {
        perror("Listen failed");
        close(sockfd);
        exit(1);
    }

    printf("Server waiting...\n");

    addrlen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen);
    if (newsockfd == -1)
    {
        printf("Accept failed");
        close(sockfd);
        exit(1);
    }

    printf("Client connected\n");

    while (1)
    {
        memset(buff, 0, sizeof(buff));
        recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recedbytes <= 0)
        {
            printf("Client disconnected\n");
            break;
        }

        printf("Client: %s\n", buff);

        if (strcmp(buff, "stop") == 0)
            break;

        memset(buff, 0, sizeof(buff));
        printf("Server: ");
        scanf("%89s", buff);

        sentbytes = send(newsockfd, buff, strlen(buff) + 1, 0);
        if (sentbytes == -1)
        {
            perror("Send failed");
            break;
        }

        if (strcmp(buff, "stop") == 0)
            break;
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
