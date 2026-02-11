#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8050
#define MAXSIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;
    char hostname[MAXSIZE], response[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    printf("Enter hostname: ");
    scanf("%1023s", hostname);

    send(sockfd, hostname, sizeof(hostname), 0);

    recv(sockfd, response, sizeof(response), 0);

    printf("DNS Response: %s\n", response);

    close(sockfd);
    return 0;
}
