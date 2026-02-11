// UDP Client - Displays socket info and exchanges hello messages
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
    socklen_t len = sizeof(client);
    char buf[BUF];

    // socket() with SOCK_DGRAM creates UDP socket (connectionless)
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(3388);  // htons() converts port to network byte order
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // inet_addr() converts "x.x.x.x" to binary
    
    // Configure and bind to specific client port
    client.sin_family = AF_INET;
    client.sin_port = htons(3389);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(sock, (struct sockaddr*)&client, sizeof(client));  // bind() attaches socket to address

    // getsockname() retrieves actual address assigned to socket
    getsockname(sock, (struct sockaddr*)&client, &len);
    
    // Display client socket information
    printf("\nClient Info:\n");
    printf("Address: %p\n", (void*)&client);  // %p prints pointer in hexadecimal
    printf("IP: %s\n", inet_ntoa(client.sin_addr));  // inet_ntoa() converts binary IP to "x.x.x.x"
    printf("Port: %d\n\n", ntohs(client.sin_port));  // ntohs() converts network to host byte order
    printf("Connected to Server.\n");

    // Send greeting message to server
    strcpy(buf, "Hello from client!");  // strcpy() copies string to buffer
    // sendto() sends datagram to specified address (UDP doesn't need connect)
    sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&server, sizeof(server));

    // Receive server response
    len = sizeof(server);
    memset(buf, 0, BUF);  // memset() fills buffer with 0s to clear it
    // recvfrom() receives datagram and stores sender address
    recvfrom(sock, buf, BUF, 0, (struct sockaddr*)&server, &len);
    printf("\nServer Response: %s\n", buf);

    close(sock);  // close() releases socket resources
    return 0;
}
