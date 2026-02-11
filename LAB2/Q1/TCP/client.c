#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1024

int main(void)
{
    int sockfd, retval;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE], filename[MAXSIZE], str1[MAXSIZE], str2[MAXSIZE];
    int option;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        printf("\nSocket Creation Error");
        exit(0);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    if (retval == -1)
    {
        printf("Connection error");
        exit(0);
    }

    printf("Enter filename: ");
    scanf("%s", filename);

    sentbytes = send(sockfd, filename, sizeof(filename), 0);

    if (sentbytes == -1)
    {
        printf("Send error");
        close(sockfd);
        exit(0);
    }

    recedbytes = recv(sockfd, buff, sizeof(buff), 0);

    if (recedbytes == -1)
    {
        close(sockfd);
        exit(0);
    }

    printf("%s\n", buff);

    if (strcmp(buff, "File not present") == 0)
    {
        close(sockfd);
        return 0;
    }

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Search\n");
        printf("2. Replace\n");
        printf("3. Reorder\n");
        printf("4. Exit\n");
        printf("Enter option: ");
        scanf("%d", &option);

        sprintf(buff, "%d", option);
        sentbytes = send(sockfd, buff, sizeof(buff), 0);

        if (option == 4)
        {
            break;
        }

        if (option == 1)
        {
            printf("Enter string to search: ");
            scanf("%s", str1);

            sentbytes = send(sockfd, str1, sizeof(str1), 0);
            recedbytes = recv(sockfd, buff, sizeof(buff), 0);

            printf("%s\n", buff);
        }
        else if (option == 2)
        {
            printf("Enter string to replace: ");
            scanf("%s", str1);
            printf("Enter replacement string: ");
            scanf("%s", str2);

            sentbytes = send(sockfd, str1, sizeof(str1), 0);
            sentbytes = send(sockfd, str2, sizeof(str2), 0);
            recedbytes = recv(sockfd, buff, sizeof(buff), 0);

            printf("%s\n", buff);
        }
        else if (option == 3)
        {
            recedbytes = recv(sockfd, buff, sizeof(buff), 0);
            printf("%s\n", buff);
        }
    }

    close(sockfd);

    return 0;
}
