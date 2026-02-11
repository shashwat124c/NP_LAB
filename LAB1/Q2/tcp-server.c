#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define BUF 1024
#define PORT 3388

int is_palindrome(char *s) {
    int len = strlen(s);
    for (int i = 0; i < len / 2; i++)
        if (s[i] != s[len - 1 - i])
            return 0;
    return 1;
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in server, client;
    socklen_t len;
    char buf[BUF], result[BUF];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    listen(sockfd, 5);

    printf("TCP Server listening on port %d...\n", PORT);

    len = sizeof(client);
    connfd = accept(sockfd, (struct sockaddr*)&client, &len);

    while (1) {
        memset(buf, 0, BUF);
        if (recv(connfd, buf, BUF, 0) <= 0)
            break;

        if (strcmp(buf, "Halt") == 0)
            break;

        int v[5] = {0};
        for (int i = 0; buf[i]; i++) {
            char c = tolower(buf[i]);
            if (c == 'a') v[0]++;
            else if (c == 'e') v[1]++;
            else if (c == 'i') v[2]++;
            else if (c == 'o') v[3]++;
            else if (c == 'u') v[4]++;
        }

        snprintf(result, BUF,
                 "%s palindrome\nLength: %lu\nVowels - A:%d E:%d I:%d O:%d U:%d",
                 is_palindrome(buf) ? "Is" : "Not",
                 strlen(buf), v[0], v[1], v[2], v[3], v[4]);

        send(connfd, result, strlen(result) + 1, 0);
    }

    close(connfd);
    close(sockfd);
    return 0;
}
