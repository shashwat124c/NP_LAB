#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1024
#define PORT 6000

/* Function to swap two characters */
void swap(char *a, char *b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

void permute(char *str, int l, int r, char *result)
{
    if (l == r)
    {
        strcat(result, str);
        strcat(result, "\n");
        return;
    }

    for (int i = l; i <= r; i++)
    {
        swap(&str[l], &str[i]);
        permute(str, l + 1, r, result);
        swap(&str[l], &str[i]);   // backtrack
    }
}

int main()
{
    int sockfd;
    socklen_t addrlen;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE];
    char result[MAXSIZE * 4];   // buffer to store permutations
    ssize_t n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    printf("UDP Permutation Server running on port %d\n", PORT);

    addrlen = sizeof(clientaddr);

    while (1)
    {
        n = recvfrom(sockfd, buff, sizeof(buff) - 1, 0,
                     (struct sockaddr *)&clientaddr, &addrlen);
        if (n <= 0)
            continue;

        buff[n] = '\0';
        if (strcmp(buff, "BYE") == 0)
            break;

        printf("Received string: %s\n", buff);

        result[0] = '\0';
        permute(buff, 0, strlen(buff) - 1, result);

        printf("\nPermutations:\n%s\n", result);

        sendto(sockfd, result, strlen(result) + 1, 0,
               (struct sockaddr *)&clientaddr, addrlen);
    }

    close(sockfd);
    return 0;
}
