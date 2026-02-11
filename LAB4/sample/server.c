#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAXBOOKS 50
#define PORT 9000

/* 1. Structure for library database */
struct book
{
    int book_id;
    char title[50];
    char author[50];
};

int main()
{
    int sockfd, newsockfd;
    socklen_t addrlen;
    struct sockaddr_in serveraddr, clientaddr;

    struct book library[MAXBOOKS];
    int count = 0, option;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* 3. Initialize server address */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);          // dynamic allowed, fixed used for simplicity
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* 4. Bind */
    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    /* 5. Listen */
    listen(sockfd, 1);
    printf("Library Server running on port %d\n", PORT);

    addrlen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen);

    while (1)
    {
        /* Receive client option */
        recv(newsockfd, &option, sizeof(option), 0);

        /* 8. Exit option */
        if (option == 4)
        {
            printf("Client disconnected\n");
            break;
        }

        /* 6. Perform database operations */

        /* ADD BOOK */
        if (option == 1)
        {
            recv(newsockfd, &library[count], sizeof(struct book), 0);
            count++;

            char msg[] = "Book added successfully";
            send(newsockfd, msg, sizeof(msg), 0);
        }

        /* SEARCH BOOK */
        else if (option == 2)
        {
            int id, found = 0;
            recv(newsockfd, &id, sizeof(id), 0);

            for (int i = 0; i < count; i++)
            {
                if (library[i].book_id == id)
                {
                    send(newsockfd, &library[i], sizeof(struct book), 0);
                    found = 1;
                    break;
                }
            }

            if (!found)
            {
                struct book empty = {-1, "", ""};
                send(newsockfd, &empty, sizeof(struct book), 0);
            }
        }

        /* DISPLAY ALL BOOKS */
        else if (option == 3)
        {
            send(newsockfd, &count, sizeof(count), 0);
            send(newsockfd, library, sizeof(struct book) * count, 0);
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
