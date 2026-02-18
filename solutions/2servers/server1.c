// SERVER 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9001
#define MAX 1024

int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len = sizeof(clientaddr);
    char buffer[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 5);

    while(1)
    {
        newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);

        if(fork() == 0)
        {
            recv(newsockfd, buffer, MAX, 0);

            FILE *fp = fopen("server1.txt", "a");
            fprintf(fp, "Client IP: %s\nMessage: %s\n\n",
                    inet_ntoa(clientaddr.sin_addr), buffer);
            fclose(fp);

            send(newsockfd, "Stored in Server 1", 20, 0);

            close(newsockfd);
            exit(0);
        }

        close(newsockfd);
    }
}
