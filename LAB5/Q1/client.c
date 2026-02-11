#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define MAX 1024

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Enter string to send: ");
    fgets(buffer, MAX, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; //Find the position of the first \n inside buffer, then replace that char with \0

    send(sockfd, buffer, strlen(buffer) + 1, 0);

    recv(sockfd, buffer, MAX, 0);
    printf("Server response: %s\n", buffer); 

    close(sockfd);
    return 0;
}
