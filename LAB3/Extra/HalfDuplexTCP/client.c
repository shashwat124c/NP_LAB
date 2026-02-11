#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;
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
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* 3. Connect to server */
    connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    printf("Connected to server\n");

    while (1)
    {
        /* 5. Read message from client console */
        printf("Client: ");
        scanf("%1023s", buff);

        send(sockfd, buff, strlen(buff) + 1, 0);

        if (strcmp(buff, "BYE") == 0)
            break;

        /* 6. Receive message from server */
        n = recv(sockfd, buff, sizeof(buff) - 1, 0);
        if (n <= 0)
            break;

        buff[n] = '\0';
        printf("Server: %s\n", buff);

        /* 7. Terminate if BYE */
        if (strcmp(buff, "BYE") == 0)
            break;
    }

    /* 8. Close socket */
    close(sockfd);
    return 0;
}
