#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8000

int main()
{
    int sockfd, newsockfd;
    socklen_t addrlen;
    struct sockaddr_in serveraddr, clientaddr;
    int option;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 1);

    printf("Server running on port %d\n", PORT);

    addrlen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen);

    while (1)
    {
        /* Receive option from client */
        recv(newsockfd, &option, sizeof(option), 0);

        if (option == 4)
            break;

        /* OPTION 1: Add & Subtract */
        if (option == 1)
        {
            int a, b, add, sub;
            recv(newsockfd, &a, sizeof(a), 0);
            recv(newsockfd, &b, sizeof(b), 0);

            add = a + b;
            sub = a - b;

            send(newsockfd, &add, sizeof(add), 0);
            send(newsockfd, &sub, sizeof(sub), 0);
        }

        /* OPTION 2: Solve linear equation ax + b = 0 */
        else if (option == 2)
        {
            float a, b, x;
            recv(newsockfd, &a, sizeof(a), 0);
            recv(newsockfd, &b, sizeof(b), 0);

            x = -b / a;
            send(newsockfd, &x, sizeof(x), 0);
        }

        /* OPTION 3: Matrix multiplication */
        else if (option == 3)
        {
            int r1, c1, r2, c2;
            int A[10][10], B[10][10], C[10][10];

            recv(newsockfd, &r1, sizeof(r1), 0);
            recv(newsockfd, &c1, sizeof(c1), 0);
            recv(newsockfd, &r2, sizeof(r2), 0);
            recv(newsockfd, &c2, sizeof(c2), 0);

            recv(newsockfd, A, sizeof(A), 0);
            recv(newsockfd, B, sizeof(B), 0);

            for (int i = 0; i < r1; i++)
                for (int j = 0; j < c2; j++)
                {
                    C[i][j] = 0;
                    for (int k = 0; k < c1; k++)
                        C[i][j] += A[i][k] * B[k][j];
                }

            send(newsockfd, C, sizeof(C), 0);
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
