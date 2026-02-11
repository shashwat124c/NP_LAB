#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXSIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;
    char filename[100];
    char buffer[MAXSIZE];
    ssize_t n;

    /* 1. Create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        exit(1);
    }

    /* 2. Initialize server address */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(3388);   // replace with server's printed port

    /* 3. Connect to server */
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Connect failed");
        exit(1);
    }

    /* 4. Send file name */
    printf("Enter file name: ");
    scanf("%99s", filename);
    send(sockfd, filename, strlen(filename) + 1, 0);

    /* 5. Receive file contents */
    printf("\n--- File Contents ---\n");
    while ((n = recv(sockfd, buffer, sizeof(buffer), 0)) > 0)
    {
        write(1, buffer, n);   // write to stdout
    }

    printf("\n--- End of File ---\n");

    /* 6. Close socket */
    close(sockfd);

    return 0;
}
