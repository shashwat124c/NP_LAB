#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DNS_PORT 8000
#define MAX 1024

int resolve_port(char *server_name)
{
    if(strcmp(server_name, "server1") == 0)
        return 9001;
    if(strcmp(server_name, "server2") == 0)
        return 9002;
    return -1;
}

int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len = sizeof(clientaddr);
    char server_name[50], message[MAX], buffer[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(DNS_PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 5);

    while(1)
    {
        newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);

        recv(newsockfd, server_name, sizeof(server_name), 0);
        recv(newsockfd, message, sizeof(message), 0);

        int port = resolve_port(server_name);

        int server_sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in target;
        target.sin_family = AF_INET;
        target.sin_port = htons(port);
        target.sin_addr.s_addr = inet_addr("127.0.0.1");

        connect(server_sock, (struct sockaddr*)&target, sizeof(target));
        send(server_sock, message, strlen(message)+1, 0);

        recv(server_sock, buffer, sizeof(buffer), 0);

        send(newsockfd, buffer, strlen(buffer)+1, 0);

        close(server_sock);
        close(newsockfd);
    }
}
