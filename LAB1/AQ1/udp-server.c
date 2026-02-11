// UDP Server - Displays client info and exchanges hello messages
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF 1024

int main() {
    int sock;
    socklen_t len;
    struct sockaddr_in server, client;
    char buf[BUF];

    // socket() with SOCK_DGRAM creates UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Configure server to listen on all interfaces
    server.sin_family = AF_INET;
    server.sin_port = htons(3388);  // htons() converts port to network byte order
    server.sin_addr.s_addr = INADDR_ANY;  // INADDR_ANY = accept from any interface (0.0.0.0)
    
    // bind() attaches socket to port
    bind(sock, (struct sockaddr*)&server, sizeof(server));
    printf("Listening on port 3388...\n");

    // Receive datagram and get client address
    len = sizeof(client);
    memset(buf, 0, BUF);  // memset() zeros out buffer
    // recvfrom() receives datagram and captures sender's address in 'client' struct
    recvfrom(sock, buf, BUF, 0, (struct sockaddr*)&client, &len);

    // Display client socket information
    printf("\nClient Info:\n");
    printf("Address: %p\n", (void*)&client);  // %p prints memory address in hex
    printf("IP: %s\n", inet_ntoa(client.sin_addr));  // inet_ntoa() converts binary IP to "x.x.x.x"
    printf("Port: %d\n\n", ntohs(client.sin_port));  // ntohs() converts network to host byte order
    printf("Client message: %s\n", buf);

    // Send greeting response back to client
    strcpy(buf, "Hello from server!");  // strcpy() copies string to buffer
    // sendto() sends datagram to specific client address
    sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&client, sizeof(client));

    close(sock);  // close() releases socket resources
    return 0;
}
