#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define MAXSIZE 1024
#define PORT 4567

int main()
{
    int sockfd;
    socklen_t addrlen;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE];
    ssize_t n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    printf("UDP Server running on port %d\n", PORT);

    addrlen = sizeof(clientaddr);

    /* 🔥 RECEIVE ONCE to get client address */
    n = recvfrom(sockfd, buff, sizeof(buff)-1, 0,
                 (struct sockaddr *)&clientaddr, &addrlen);
    buff[n] = '\0';
    printf("Client: %s\n", buff);

    /* NOW fork */
    if (fork() == 0)
    {
        /* CHILD: receive */
        while (1)
        {
            n = recvfrom(sockfd, buff, sizeof(buff)-1, 0,
                         (struct sockaddr *)&clientaddr, &addrlen);
            if (n <= 0) continue;

            buff[n] = '\0';
            printf("\nClient: %s\n", buff);

            if (strcmp(buff, "BYE") == 0)
            {
                kill(getppid(), SIGKILL);
                break;
            }
        }
    }
    else
    {
        /* PARENT: send */
        while (1)
        {
            printf("Server: ");
            scanf("%1023s", buff);

            sendto(sockfd, buff, strlen(buff)+1, 0,
                   (struct sockaddr *)&clientaddr, addrlen);

            if (strcmp(buff, "BYE") == 0)
                break;
        }
    }

    close(sockfd);
    return 0;
}
