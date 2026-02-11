#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXSIZE 4096

int main(void)
{
    int sockfd;
    ssize_t recedbytes;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE], filename[MAXSIZE], filecontent[MAXSIZE * 10];
    int filesize, alphabets, lines, spaces, digits, others;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3391);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    while (1)
    {
        printf("Enter filename (or 'stop'): ");
        scanf("%4095s", filename);

        send(sockfd, filename, strlen(filename)+1, 0);   // FIX

        if (strcmp(filename, "stop") == 0)
            break;

        recedbytes = recv(sockfd, buff, sizeof(buff)-1, 0);
        buff[recedbytes] = '\0';                         // FIX

        if (strcmp(buff, "File not present") == 0)
        {
            printf("%s\n", buff);
            continue;
        }

        strcpy(filecontent, buff);

        recv(sockfd, buff, sizeof(buff)-1, 0);
        filesize = atoi(buff);

        recv(sockfd, buff, sizeof(buff)-1, 0);
        alphabets = atoi(buff);

        recv(sockfd, buff, sizeof(buff)-1, 0);
        lines = atoi(buff);

        recv(sockfd, buff, sizeof(buff)-1, 0);
        spaces = atoi(buff);

        recv(sockfd, buff, sizeof(buff)-1, 0);
        digits = atoi(buff);

        recv(sockfd, buff, sizeof(buff)-1, 0);
        others = atoi(buff);

        printf("\nFile content:\n%s\n", filecontent);
        printf("File size: %d\n", filesize);
        printf("Alphabets: %d\nLines: %d\nSpaces: %d\nDigits: %d\nOthers: %d\n",
               alphabets, lines, spaces, digits, others);
    }

    close(sockfd);
    return 0;
}
