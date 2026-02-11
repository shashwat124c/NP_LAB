#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF 1024

int main() {
    int sock;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(server);
    char buf[BUF];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    server.sin_family = AF_INET;
    server.sin_port = htons(3388); 
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  
    
    client.sin_family = AF_INET;
    client.sin_port = htons(3389);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    bind(sock, (struct sockaddr*)&client, sizeof(client));

    printf("Ready.\n");
    while (1) {
        printf("Enter string ('Halt' to exit): ");
        scanf(" %[^\n]", buf);
        
        sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&server, sizeof(server));

        if (strcmp(buf, "Halt") == 0) break;

        memset(buf, 0, BUF);  
        recvfrom(sock, buf, BUF, 0, (struct sockaddr*)&server, &len);
        printf("Result:\n%s\n", buf);
    }
    
    close(sock);
    return 0;
}
