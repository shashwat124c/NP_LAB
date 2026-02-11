#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define PORT 9000

struct book
{
    int book_id;
    char title[50];
    char author[50];
};

int main()
{
    int sockfd, option;
    struct sockaddr_in serveraddr;
    struct book b;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* 3. Initialize server address */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* 4. Connect to server */
    connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    while (1)
    {
        printf("\nLIBRARY MENU\n");
        printf("1. Add Book\n");
        printf("2. Search Book\n");
        printf("3. Display All Books\n");
        printf("4. Exit\n");
        printf("Enter option: ");
        scanf("%d", &option);

        send(sockfd, &option, sizeof(option), 0);

        if (option == 4)
            break;

        /* ADD BOOK */
        if (option == 1)
        {
            printf("Enter Book ID: ");
            scanf("%d", &b.book_id);
            printf("Enter Title: ");
            scanf("%s", b.title);
            printf("Enter Author: ");
            scanf("%s", b.author);

            send(sockfd, &b, sizeof(struct book), 0);

            char msg[100];
            recv(sockfd, msg, sizeof(msg), 0);
            printf("%s\n", msg);
        }

        /* SEARCH BOOK */
        else if (option == 2)
        {
            int id;
            printf("Enter Book ID to search: ");
            scanf("%d", &id);

            send(sockfd, &id, sizeof(id), 0);
            recv(sockfd, &b, sizeof(struct book), 0);

            if (b.book_id == -1)
                printf("Book not found\n");
            else
                printf("Book Found: %d %s %s\n", b.book_id, b.title, b.author);
        }

        /* DISPLAY ALL BOOKS */
        else if (option == 3)
        {
            int count;
            recv(sockfd, &count, sizeof(count), 0);

            struct book list[50];
            recv(sockfd, list, sizeof(struct book) * count, 0);

            printf("\nLibrary Books:\n");
            for (int i = 0; i < count; i++)
                printf("%d %s %s\n",
                       list[i].book_id,
                       list[i].title,
                       list[i].author);
        }
    }

    close(sockfd);
    return 0;
}
