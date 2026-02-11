#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define MAXSIZE 1024

int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t addrlen;
    char filename[100];
    char buffer[MAXSIZE];
    int fd;
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
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(3388);   // dynamically assigned port

    /* 3. Bind */
    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Bind failed");
        exit(1);
    }

    /* Print assigned port */
    addrlen = sizeof(serveraddr);
    getsockname(sockfd, (struct sockaddr *)&serveraddr, &addrlen);
    printf("Server running on port %d\n", ntohs(serveraddr.sin_port));

    /* 4. Listen */
    listen(sockfd, 1);

    /* 5. Accept client */
    addrlen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen);
    if (newsockfd == -1)
    {
        perror("Accept failed");
        exit(1);
    }

    /* 6. Receive file name */
    memset(filename, 0, sizeof(filename));
    recv(newsockfd, filename, sizeof(filename), 0);
    printf("Client requested file: %s\n", filename);

    /* 7. Open file */
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("File open failed");
        close(newsockfd);
        close(sockfd);
        exit(1);
    }

    /* 8. Read file and send contents */
    while ((n = read(fd, buffer, sizeof(buffer))) > 0)
    {
        send(newsockfd, buffer, n, 0);
    }

    /* 9. Close file and sockets */
    close(fd);
    close(newsockfd);
    close(sockfd);

    return 0;
}
