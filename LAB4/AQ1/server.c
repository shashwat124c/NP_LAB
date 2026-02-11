/* SERVER PROGRAM */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9095
#define MAX 1024
#define MAXBOOKS 50

struct Book {
    char title[50];
    char author[50];
    int accession;
    int pages;
    char publisher[50];
};

int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    int choice, count = 0;
    struct Book library[MAXBOOKS], b;
    char buffer[MAX];
    int found;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 5);

    printf("Book Server running on port %d...\n", PORT);

    len = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &len);

    while (1)
    {
        recv(newsockfd, &choice, sizeof(choice), 0);

        /* EXIT */
        if (choice == 5)
            break;

        /* INSERT */
        if (choice == 1)
        {
            recv(newsockfd, &b, sizeof(b), 0);
            library[count++] = b;
            send(newsockfd, "Book inserted successfully", 27, 0);
        }

        /* DELETE */
        else if (choice == 2)
        {
            int acc;
            recv(newsockfd, &acc, sizeof(acc), 0);
            found = 0;

            for (int i = 0; i < count; i++)
            {
                if (library[i].accession == acc)
                {
                    library[i] = library[count - 1];
                    count--;
                    found = 1;
                    break;
                }
            }

            if (found)
                send(newsockfd, "Book deleted", 13, 0);
            else
                send(newsockfd, "Book not found", 15, 0);
        }

        /* DISPLAY ALL */
        else if (choice == 3)
        {
            send(newsockfd, &count, sizeof(count), 0);
            for (int i = 0; i < count; i++)
                send(newsockfd, &library[i], sizeof(struct Book), 0);
        }

        /* SEARCH */
        else if (choice == 4)
        {
            int opt;
            char key[50];
            recv(newsockfd, &opt, sizeof(opt), 0);
            recv(newsockfd, key, sizeof(key), 0);

            found = 0;
            for (int i = 0; i < count; i++)
            {
                if ((opt == 1 && strcmp(library[i].title, key) == 0) ||
                    (opt == 2 && strcmp(library[i].author, key) == 0))
                {
                    send(newsockfd, &library[i], sizeof(struct Book), 0);
                    found = 1;
                }
            }

            if (!found)
            {
                struct Book empty = { "", "", -1, 0, "" };
                send(newsockfd, &empty, sizeof(empty), 0);
            }
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
