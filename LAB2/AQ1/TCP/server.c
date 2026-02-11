#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXSIZE 4096

int main(void)
{
    int sockfd, newsockfd;
    socklen_t actuallen;
    ssize_t recedbytes;
    struct sockaddr_in serveraddr, clientaddr;

    char buff[MAXSIZE], filename[MAXSIZE], filecontent[MAXSIZE * 10];
    FILE *fp;
    int alphabets, digits, spaces, lines, others, filesize;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3391);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);   // FIX

    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 1);

    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &actuallen);

    while (1)
    {
        recedbytes = recv(newsockfd, filename, sizeof(filename)-1, 0);
        if (recedbytes <= 0) break;
        filename[recedbytes] = '\0';                 // FIX

        if (strcmp(filename, "stop") == 0)
            break;

        fp = fopen(filename, "r");
        if (!fp)
        {
            strcpy(buff, "File not present");
            send(newsockfd, buff, strlen(buff)+1, 0);   // FIX
            continue;
        }

        fseek(fp, 0, SEEK_END);
        filesize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        fread(filecontent, 1, filesize, fp);
        filecontent[filesize] = '\0';
        fclose(fp);

        alphabets = digits = spaces = lines = others = 0;

        for (int i = 0; i < filesize; i++)
        {
            if (isalpha(filecontent[i])) alphabets++;
            else if (isdigit(filecontent[i])) digits++;
            else if (filecontent[i] == ' ') spaces++;
            else if (filecontent[i] == '\n') lines++;
            else others++;
        }

        send(newsockfd, filecontent, filesize+1, 0);   // FIX

        sprintf(buff, "%d", filesize);
        send(newsockfd, buff, strlen(buff)+1, 0);

        sprintf(buff, "%d", alphabets);
        send(newsockfd, buff, strlen(buff)+1, 0);

        sprintf(buff, "%d", lines);
        send(newsockfd, buff, strlen(buff)+1, 0);

        sprintf(buff, "%d", spaces);
        send(newsockfd, buff, strlen(buff)+1, 0);

        sprintf(buff, "%d", digits);
        send(newsockfd, buff, strlen(buff)+1, 0);

        sprintf(buff, "%d", others);
        send(newsockfd, buff, strlen(buff)+1, 0);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
