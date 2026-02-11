#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 3388
#define BUF 1024

int main() {
    int sock;
    struct sockaddr_in server;
    char buf[BUF], choice;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);  
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  
    
    connect(sock, (struct sockaddr*)&server, sizeof(server));
    printf("Connected to Server.\n");

    while (1) {
        printf("\n1.Search 2.Sort Asc 3.Sort Desc 4.Odd/Even 5.Exit\nChoice: ");
        scanf("%s", buf);
        choice = buf[0];
        
        
        send(sock, buf, strlen(buf), 0);

        if (choice == '5') break;

        printf("Count: "); 
        scanf("%s", buf); 
        send(sock, buf, strlen(buf), 0);
        
        printf("Numbers: "); 
        scanf(" %[^\n]", buf);  // %[^\n] reads entire line including spaces until newline
        send(sock, buf, strlen(buf), 0);
        
        if (choice == '1') {
            printf("Search for: "); 
            scanf("%s", buf); 
            send(sock, buf, strlen(buf), 0);
        }

        memset(buf, 0, BUF);  
        recv(sock, buf, BUF, 0);  
        printf("Result: %s\n", buf);
    }
    
    close(sock);
    return 0;
}
