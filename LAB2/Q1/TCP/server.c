#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1024

int char_cmp(const void *a, const void *b) {
    return *(char*)a - *(char*)b;  // Cast void* to char* then dereference
}

int main(void)
{
    int sockfd, newsockfd, retval;
    socklen_t actuallen;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE], filename[MAXSIZE], str1[MAXSIZE], str2[MAXSIZE], filecontent[MAXSIZE * 10];
    int option, count;
    FILE *fp;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        printf("\nSocket creation error");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = htons(INADDR_ANY);

    retval = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    if (retval == -1)
    {
        printf("Binding error");
        close(sockfd);
        exit(0);
    }

    retval = listen(sockfd, 1);

    if (retval == -1)
    {
        close(sockfd);
        exit(0);
    }

    actuallen = sizeof(clientaddr);

    newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &actuallen);

    if (newsockfd == -1)
    {
        close(sockfd);
        exit(0);
    }

    recedbytes = recv(newsockfd, filename, sizeof(filename), 0);

    if (recedbytes == -1)
    {
        close(sockfd);
        close(newsockfd);
        exit(0);
    }

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        strcpy(buff, "File not present");
        sentbytes = send(newsockfd, buff, sizeof(buff), 0);
        close(sockfd);
        close(newsockfd);
        return 0;
    }

    fread(filecontent, sizeof(char), sizeof(filecontent), fp);
    fclose(fp);

    strcpy(buff, "File present");
    sentbytes = send(newsockfd, buff, sizeof(buff), 0);

    while (1)
    {
        recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1) break;

        option = atoi(buff); 

        if (option == 4)
        {
            break;
        }

        if (option == 1)
        {
            recedbytes = recv(newsockfd, str1, sizeof(str1), 0);
            if (recedbytes == -1) break;

            count = 0;
            char *pos = filecontent;
            while ((pos = strstr(pos, str1)) != NULL) //strstr searches for substring within string
            {
                count++;
                pos += strlen(str1);
            }

            if (count > 0)
            {
                sprintf(buff, "%d", count);
            }
            else
            {
                strcpy(buff, "String not found");
            }

            sentbytes = send(newsockfd, buff, sizeof(buff), 0);
        }
        else if (option == 2)
        {
            recedbytes = recv(newsockfd, str1, sizeof(str1), 0);
            if (recedbytes == -1) break;

            recedbytes = recv(newsockfd, str2, sizeof(str2), 0);
            if (recedbytes == -1) break;

            /* Check if str1 exists in file */
            if (strstr(filecontent, str1) == NULL)
            {
                strcpy(buff, "String not found");
            }
            else
            {
                char *pos;
                while ((pos = strstr(filecontent, str1)) != NULL)
                {
                    memmove(pos + strlen(str2), pos + strlen(str1), strlen(pos + strlen(str1)) + 1);
                    memcpy(pos, str2, strlen(str2));
                } //source and destination memory overlap, and memcpy() would corrupt data, so use memmove

                fp = fopen(filename, "w");
                fwrite(filecontent, sizeof(char), strlen(filecontent), fp);
                fclose(fp);

                strcpy(buff, "String replaced");
            }

            sentbytes = send(newsockfd, buff, sizeof(buff), 0);
        }
        else if (option == 3)
        {
            int len = strlen(filecontent);

            qsort(filecontent, len, sizeof(char), char_cmp);

            fp = fopen(filename, "w");
            fwrite(filecontent, sizeof(char), len, fp);
            fclose(fp);

            strcpy(buff, "File reordered");
            sentbytes = send(newsockfd, buff, sizeof(buff), 0);
        }
    }

    close(sockfd);
    close(newsockfd);

    return 0;
}
