#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1024
#define PORT 3456   // same fixed port number

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

    while (1)
    {
        /* 3. Read message from client console */
        printf("Client: ");
        scanf("%1023s", buff);

        sendto(sockfd, buff, strlen(buff) + 1, 0,
               (struct sockaddr *)&serveraddr, addrlen);

        if (strcmp(buff, "BYE") == 0)
            break;

        /* 4. Receive message from server */
        n = recvfrom(sockfd, buff, sizeof(buff) - 1, 0,
                     NULL, NULL);
        if (n <= 0)
            continue;

        buff[n] = '\0';
        printf("Server: %s\n", buff);

        if (strcmp(buff, "BYE") == 0)
            break;
    }

    close(sockfd);
    return 0;
}
