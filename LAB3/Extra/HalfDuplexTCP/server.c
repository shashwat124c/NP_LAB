#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1024

int main()
{
    int sockfd, newsockfd;
    socklen_t addrlen;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE];
    ssize_t n;

    /* 1. Create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        exit(1);
    }

    /* 2. Initialize server address */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(45427);             
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* 3. Bind */
    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    /* Print assigned port */
    addrlen = sizeof(serveraddr);
    getsockname(sockfd, (struct sockaddr *)&serveraddr, &addrlen);
    printf("Server running on port %d\n", ntohs(serveraddr.sin_port));

    /* 4. Listen */
    listen(sockfd, 1);
    printf("Waiting for client...\n");

    /* 5. Accept connection */
    addrlen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen);

    printf("Client connected\n");

    while (1)
    {
        /* 6. Receive message from client */
        n = recv(newsockfd, buff, sizeof(buff) - 1, 0);
        if (n <= 0)
            break;

        buff[n] = '\0';
        printf("Client: %s\n", buff);

        /* 8. Terminate if BYE */
        if (strcmp(buff, "BYE") == 0)
            break;

        /* 7. Read message from server console */
        printf("Server: ");
        scanf("%1023s", buff);

        send(newsockfd, buff, strlen(buff) + 1, 0);

        if (strcmp(buff, "BYE") == 0)
            break;
    }

    /* 9. Close sockets */
    close(newsockfd);
    close(sockfd);
    return 0;
}
