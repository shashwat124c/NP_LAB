#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define MAXSIZE 1024
#define FILEBUF 8192

/* qsort comparison */
int char_cmp(const void *a, const void *b)
{
    return (*(char *)a - *(char *)b);
}

int main(void)
{
    int sockfd, retval;
    socklen_t actuallen;
    ssize_t recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;

    char buff[MAXSIZE];
    char filename[MAXSIZE];
    char str1[MAXSIZE], str2[MAXSIZE];
    char filecontent[FILEBUF];
    char temp[FILEBUF];

    int option, count;
    FILE *fp;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        return 0;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3389);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        perror("Bind failed");
        close(sockfd);
        return 0;
    }

    actuallen = sizeof(clientaddr);

    /* Receive filename */
    recedbytes = recvfrom(sockfd, filename, sizeof(filename) - 1, 0,
                           (struct sockaddr *)&clientaddr, &actuallen);
    if (recedbytes <= 0)
    {
        close(sockfd);
        return 0;
    }
    filename[recedbytes] = '\0';

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        strcpy(buff, "File not present");
        sendto(sockfd, buff, strlen(buff) + 1, 0,
               (struct sockaddr *)&clientaddr, actuallen);
        close(sockfd);
        return 0;
    }

    size_t flen = fread(filecontent, 1, FILEBUF - 1, fp);
    filecontent[flen] = '\0';
    fclose(fp);

    strcpy(buff, "File present");
    sendto(sockfd, buff, strlen(buff) + 1, 0,
           (struct sockaddr *)&clientaddr, actuallen);

    while (1)
    {
        recedbytes = recvfrom(sockfd, buff, sizeof(buff) - 1, 0,
                               (struct sockaddr *)&clientaddr, &actuallen);
        if (recedbytes <= 0)
            break;

        buff[recedbytes] = '\0';
        option = atoi(buff);

        if (option == 4)
            break;

        /* OPTION 1: COUNT */
        if (option == 1)
        {
            recedbytes = recvfrom(sockfd, str1, sizeof(str1) - 1, 0,
                                   (struct sockaddr *)&clientaddr, &actuallen);
            if (recedbytes <= 0) break;
            str1[recedbytes] = '\0';

            count = 0;
            char *pos = filecontent;
            while ((pos = strstr(pos, str1)) != NULL)
            {
                count++;
                pos += strlen(str1);
            }

            if (count > 0)
                sprintf(buff, "%d", count);
            else
                strcpy(buff, "String not found");

            sendto(sockfd, buff, strlen(buff) + 1, 0,
                   (struct sockaddr *)&clientaddr, actuallen);
        }

        /* OPTION 2: REPLACE */
        else if (option == 2)
        {
            recvfrom(sockfd, str1, sizeof(str1) - 1, 0,
                     (struct sockaddr *)&clientaddr, &actuallen);
            recvfrom(sockfd, str2, sizeof(str2) - 1, 0,
                     (struct sockaddr *)&clientaddr, &actuallen);

            str1[sizeof(str1) - 1] = '\0';
            str2[sizeof(str2) - 1] = '\0';

            if (strstr(filecontent, str1) == NULL)
            {
                strcpy(buff, "String not found");
            }
            else
            {
                temp[0] = '\0';
                char *start = filecontent;
                char *p;

                while ((p = strstr(start, str1)) != NULL)
                {
                    strncat(temp, start, p - start);
                    strcat(temp, str2);
                    start = p + strlen(str1);
                }
                strcat(temp, start);

                strcpy(filecontent, temp);

                fp = fopen(filename, "w");
                fwrite(filecontent, 1, strlen(filecontent), fp);
                fclose(fp);

                strcpy(buff, "String replaced");
            }

            sendto(sockfd, buff, strlen(buff) + 1, 0,
                   (struct sockaddr *)&clientaddr, actuallen);
        }

        /* OPTION 3: SORT */
        else if (option == 3)
        {
            size_t len = strlen(filecontent);
            qsort(filecontent, len, sizeof(char), char_cmp);

            fp = fopen(filename, "w");
            fwrite(filecontent, 1, len, fp);
            fclose(fp);

            strcpy(buff, "File reordered");
            sendto(sockfd, buff, strlen(buff) + 1, 0,
                   (struct sockaddr *)&clientaddr, actuallen);
        }
    }

    close(sockfd);
    return 0;
}
