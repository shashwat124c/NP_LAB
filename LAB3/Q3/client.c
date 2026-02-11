#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1024
#define PORT 7000

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    printf("Input string: ");
    scanf("%1023s", buff);

    send(sockfd, buff, strlen(buff) + 1, 0);

    // Child process output
    recv(sockfd, buff, sizeof(buff) - 1, 0);
    printf("%s\n", buff);

    // Parent process output 
    recv(sockfd, buff, sizeof(buff) - 1, 0);
    printf("%s\n", buff);

    close(sockfd);
    return 0;
}
