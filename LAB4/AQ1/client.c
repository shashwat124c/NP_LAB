/* CLIENT PROGRAM */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9095

struct Book {
    char title[50];
    char author[50];
    int accession;
    int pages;
    char publisher[50];
};

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;
    int choice, n, opt;
    struct Book b;
    char msg[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    while (1)
    {
        printf("\n1.Insert Book\n2.Delete Book\n3.Display All\n4.Search\n5.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        send(sockfd, &choice, sizeof(choice), 0);

        if (choice == 5)
            break;

        /* INSERT */
        if (choice == 1)
        {
            printf("Title: "); scanf("%s", b.title);
            printf("Author: "); scanf("%s", b.author);
            printf("Accession No: "); scanf("%d", &b.accession);
            printf("Pages: "); scanf("%d", &b.pages);
            printf("Publisher: "); scanf("%s", b.publisher);

            send(sockfd, &b, sizeof(b), 0);
            recv(sockfd, msg, sizeof(msg), 0);
            printf("%s\n", msg);
        }

        /* DELETE */
        else if (choice == 2)
        {
            printf("Enter accession number: ");
            scanf("%d", &b.accession);
            send(sockfd, &b.accession, sizeof(int), 0);
            recv(sockfd, msg, sizeof(msg), 0);
            printf("%s\n", msg);
        }

        /* DISPLAY */
        else if (choice == 3)
        {
            recv(sockfd, &n, sizeof(n), 0);
            for (int i = 0; i < n; i++)
            {
                recv(sockfd, &b, sizeof(b), 0);
                printf("%s %s %d %d %s\n",
                       b.title, b.author, b.accession, b.pages, b.publisher);
            }
        }

        /* SEARCH */
        else if (choice == 4)
        {
            printf("1.Search by Title\n2.Search by Author\nEnter option: ");
            scanf("%d", &opt);
            send(sockfd, &opt, sizeof(opt), 0);

            printf("Enter search key: ");
            scanf("%s", msg);
            send(sockfd, msg, sizeof(msg), 0);

            while (1)
            {
                recv(sockfd, &b, sizeof(b), 0);
                if (b.accession == -1)
                {
                    printf("No matching books found\n");
                    break;
                }
                printf("%s %s %d %d %s\n",
                       b.title, b.author, b.accession, b.pages, b.publisher);
                if (opt == 1) break;
            }
        }
    }

    close(sockfd);
    return 0;
}
