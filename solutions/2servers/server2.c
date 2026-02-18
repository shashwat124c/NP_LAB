// SERVER 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9002
#define MAX 1024

unsigned int crc32(const char *data)
{
    unsigned int crc = 0xFFFFFFFF;

    while (*data)
    {
        crc ^= (unsigned char)(*data++);
        
        for (int i = 0; i < 8; i++)
        {
            if (crc & 1)
            {
                crc = crc >> 1;
                crc = crc ^ 0xEDB88320;
            }
            else
            {
                crc = crc >> 1;
            }
        }
    }

    return ~crc;
}


int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr;
    socklen_t len;
    char buffer[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 5);

    while(1)
    {
        newsockfd = accept(sockfd, NULL, NULL);

        if(fork() == 0)
        {
            recv(newsockfd, buffer, MAX, 0);

            // Child 1: Edit + Store
            if(fork() == 0)
            {
                strcat(buffer, " [Edited]");
                FILE *fp = fopen("server2.txt", "a");
                fprintf(fp, "Edited Message: %s\n", buffer);
                fclose(fp);
                exit(0);
            }

            // Child 2: CRC32
            unsigned int crc = crc32(buffer);
            char reply[MAX];
            sprintf(reply, "Message: %s\nCRC32: %u", buffer, crc);

            send(newsockfd, reply, strlen(reply)+1, 0);
            close(newsockfd);
            exit(0);
        }

        close(newsockfd);
    }
}
