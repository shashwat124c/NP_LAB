#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define BUF 1024
#define PORT 3388

int main() {
    int sock;
    struct sockaddr_in server;
    char buf[BUF];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    printf("Connected to TCP server.\n");

    while (1) {
        printf("Enter string ('Halt' to exit): ");
        scanf(" %[^\n]", buf);

        send(sock, buf, strlen(buf) + 1, 0);

        if (strcmp(buf, "Halt") == 0)
            break;

        memset(buf, 0, BUF);
        recv(sock, buf, BUF, 0);
        printf("Result:\n%s\n", buf);
    }

    close(sock);
    return 0;
}
