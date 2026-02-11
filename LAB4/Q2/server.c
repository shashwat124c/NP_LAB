#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8050
#define MAXSIZE 1024

int main()
{
    int sockfd, newsockfd;
    socklen_t addrlen;
    struct sockaddr_in serveraddr, clientaddr;
    char hostname[MAXSIZE], response[MAXSIZE];
    FILE *fp;
    char file_host[100], file_ip[100];
    int found = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    listen(sockfd, 1);
    printf("DNS Server running on port %d\n", PORT);

    addrlen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen);

    // Receive hostname from client 
    recv(newsockfd, hostname, sizeof(hostname), 0);
    hostname[strlen(hostname)] = '\0';

    // Open DNS database
    fp = fopen("database.txt", "r");

    if (fp == NULL)
    {
        strcpy(response, "Database not found");
        send(newsockfd, response, sizeof(response), 0);
    }
    else
    {
        // Iterative search 
        while (fscanf(fp, "%s %s", file_host, file_ip) != EOF)
        {
            if (strcmp(file_host, hostname) == 0)
            {
                strcpy(response, file_ip);
                found = 1;
                break;
            }
        }

        fclose(fp);

        // Send response 
        if (found)
            send(newsockfd, response, sizeof(response), 0);
        else
            send(newsockfd, "Domain not found", sizeof(response), 0);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
