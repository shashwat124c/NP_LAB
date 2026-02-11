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
    socklen_t actuallen;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE], filename[MAXSIZE], str1[MAXSIZE], str2[MAXSIZE];
    int option;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("\nSocket Creation Error");
        exit(0);
    }

    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(3390);
    clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3389);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = bind(sockfd, (struct sockaddr*)&clientaddr, sizeof(clientaddr));

    if (retval == -1)
    {
        printf("Binding error");
        close(sockfd);
        exit(0);
    }

    actuallen = sizeof(serveraddr);

    printf("Enter filename: ");
    scanf("%s", filename);

    sentbytes = sendto(sockfd, filename, sizeof(filename), 0, (struct sockaddr*)&serveraddr, actuallen);

    if (sentbytes == -1)
    {
        printf("Send error");
        close(sockfd);
        exit(0);
    }

    recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &actuallen);

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
        sentbytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, actuallen);

        if (option == 4)
        {
            break;
        }

        if (option == 1)
        {
            printf("Enter string to search: ");
            scanf("%s", str1);

            sentbytes = sendto(sockfd, str1, sizeof(str1), 0, (struct sockaddr*)&serveraddr, actuallen);
            recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &actuallen);

            printf("%s\n", buff);
        }
        else if (option == 2)
        {
            printf("Enter string to replace: ");
            scanf("%s", str1);
            printf("Enter replacement string: ");
            scanf("%s", str2);

            sentbytes = sendto(sockfd, str1, sizeof(str1), 0, (struct sockaddr*)&serveraddr, actuallen);
            sentbytes = sendto(sockfd, str2, sizeof(str2), 0, (struct sockaddr*)&serveraddr, actuallen);
            recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &actuallen);

            printf("%s\n", buff);
        }
        else if (option == 3)
        {
            recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&serveraddr, &actuallen);
            printf("%s\n", buff);
        }
    }

    close(sockfd);

    return 0;
}
