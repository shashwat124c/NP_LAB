#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1024
#define PORT 3456   // fixed port number

int main()
{
    int sockfd;
    socklen_t addrlen;
    struct sockaddr_in serveraddr, clientaddr;
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
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* 3. Bind socket */
    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    printf("UDP Server running on port %d\n", PORT);

    addrlen = sizeof(clientaddr);

    while (1)
    {
        /* 4. Receive message from client */
        n = recvfrom(sockfd, buff, sizeof(buff) - 1, 0,
                     (struct sockaddr *)&clientaddr, &addrlen);
        if (n <= 0)
            continue;

        buff[n] = '\0';
        printf("Client: %s\n", buff);

        /* 5. Terminate if BYE */
        if (strcmp(buff, "BYE") == 0)
            break;

        /* 6. Read reply from server console */
        printf("Server: ");
        scanf("%1023s", buff);

        sendto(sockfd, buff, strlen(buff) + 1, 0,
               (struct sockaddr *)&clientaddr, addrlen);

        if (strcmp(buff, "BYE") == 0)
            break;
    }

    close(sockfd);
    return 0;
}
