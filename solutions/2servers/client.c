#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DNS_PORT 8000
#define MAX 1024

int main()
{
    int sock;
    struct sockaddr_in serveraddr;
    char server_name[50], message[MAX], response[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(DNS_PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    printf("Enter server (server1/server2): ");
    scanf("%s", server_name);

    printf("Enter message: ");
    scanf(" %[^\n]", message);

    send(sock, server_name, strlen(server_name)+1, 0);
    send(sock, message, strlen(message)+1, 0);

    recv(sock, response, sizeof(response), 0);

    printf("Response:\n%s\n", response);

    close(sock);
}
