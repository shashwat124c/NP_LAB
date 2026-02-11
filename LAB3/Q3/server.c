#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXSIZE 1024
#define PORT 7000

void sort_asc(char *s)
{
    int n = strlen(s);
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (s[i] > s[j])
            {
                char t = s[i];
                s[i] = s[j];
                s[j] = t;
            }
}

void sort_desc(char *s)
{
    int n = strlen(s);
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (s[i] < s[j])
            {
                char t = s[i];
                s[i] = s[j];
                s[j] = t;
            }
}

int main()
{
    int sockfd, newsockfd;
    socklen_t addrlen;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 1);

    printf("Server listening on port %d\n", PORT);

    addrlen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen);

    recv(newsockfd, buff, sizeof(buff) - 1, 0);
    buff[strlen(buff)] = '\0';

    if (fork() == 0)
    {
        // Child-sort digits ascending 
        char digits[MAXSIZE] = "";
        char msg[MAXSIZE * 2];


        for (int i = 0; buff[i]; i++)
            if (isdigit(buff[i]))
                strncat(digits, &buff[i], 1); //appends part of a string to the end of another

        sort_asc(digits);

        snprintf(msg, sizeof(msg),
                 "Output at the child process of the server (PID %d): %s",
                 getpid(), digits);

        send(newsockfd, msg, strlen(msg) + 1, 0);
        exit(0);
    }
    else
    {
        // Parent- sort characters descending 
        char chars[MAXSIZE] = "";
        char msg[MAXSIZE * 2];


        for (int i = 0; buff[i]; i++)
            if (isalpha(buff[i]))
                strncat(chars, &buff[i], 1);

        sort_desc(chars);

        snprintf(msg, sizeof(msg),
                 "Output at the parent process of the server (PID %d): %s",
                 getpid(), chars);

        send(newsockfd, msg, strlen(msg) + 1, 0);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
