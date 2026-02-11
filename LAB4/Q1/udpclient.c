/* UDP CLIENT */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define MAX 2048

struct Request {
    int choice;
    char detail[50];
};

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    struct Request req;
    char buffer[MAX];
    socklen_t addr_len = sizeof(server_addr);
    int n;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("\n--- Student Info System (UDP) ---\n");
    printf("1. Registration Number\n");
    printf("2. Student Name\n");
    printf("3. Subject Code\n");
    printf("Enter choice: ");
    scanf("%d", &req.choice);
    getchar();   // clear newline

    printf("Enter data: ");
    fgets(req.detail, sizeof(req.detail), stdin);
    req.detail[strcspn(req.detail, "\n")] = '\0';

    /* Send request */
    sendto(sock, &req, sizeof(req), 0,
           (struct sockaddr *)&server_addr, addr_len);

    /* Receive response */
    n = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                 (struct sockaddr *)&server_addr, &addr_len);
    buffer[n] = '\0';

    printf("\n--- Server Response ---\n%s\n", buffer);

    close(sock);
    return 0;
}
