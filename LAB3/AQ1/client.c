#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define PORT 8000

int main()
{
    int sockfd, option;
    struct sockaddr_in serveraddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    while (1)
    {
        printf("\nMENU\n");
        printf("1. Add/Subtract two integers\n");
        printf("2. Find value of x in linear equation ax + b = 0\n");
        printf("3. Multiply two matrices\n");
        printf("4. Exit\n");
        printf("Enter option: ");
        scanf("%d", &option);

        send(sockfd, &option, sizeof(option), 0);

        if (option == 4)
            break;

        /* OPTION 1 */
        if (option == 1)
        {
            int a, b, add, sub;
            printf("Enter two integers: ");
            scanf("%d %d", &a, &b);

            send(sockfd, &a, sizeof(a), 0);
            send(sockfd, &b, sizeof(b), 0);

            recv(sockfd, &add, sizeof(add), 0);
            recv(sockfd, &sub, sizeof(sub), 0);

            printf("Addition: %d\n", add);
            printf("Subtraction: %d\n", sub);
        }

        /* OPTION 2 */
        else if (option == 2)
        {
            float a, b, x;
            printf("Enter a and b (ax + b = 0): ");
            scanf("%f %f", &a, &b);

            send(sockfd, &a, sizeof(a), 0);
            send(sockfd, &b, sizeof(b), 0);

            recv(sockfd, &x, sizeof(x), 0);
            printf("Value of x = %.2f\n", x);
        }

        /* OPTION 3 */
        else if (option == 3)
        {
            int r1, c1, r2, c2;
            int A[10][10], B[10][10], C[10][10];

            printf("Enter rows and columns of Matrix A: ");
            scanf("%d %d", &r1, &c1);
            printf("Enter rows and columns of Matrix B: ");
            scanf("%d %d", &r2, &c2);

            if (c1 != r2)
            {
                printf("Matrix multiplication not possible\n");
                continue;
            }

            printf("Enter Matrix A:\n");
            for (int i = 0; i < r1; i++)
                for (int j = 0; j < c1; j++)
                    scanf("%d", &A[i][j]);

            printf("Enter Matrix B:\n");
            for (int i = 0; i < r2; i++)
                for (int j = 0; j < c2; j++)
                    scanf("%d", &B[i][j]);

            send(sockfd, &r1, sizeof(r1), 0);
            send(sockfd, &c1, sizeof(c1), 0);
            send(sockfd, &r2, sizeof(r2), 0);
            send(sockfd, &c2, sizeof(c2), 0);

            send(sockfd, A, sizeof(A), 0);
            send(sockfd, B, sizeof(B), 0);

            recv(sockfd, C, sizeof(C), 0);

            printf("Resultant Matrix:\n");
            for (int i = 0; i < r1; i++)
            {
                for (int j = 0; j < c2; j++)
                    printf("%d ", C[i][j]);
                printf("\n");
            }
        }
    }

    close(sockfd);
    return 0;
}
