// TCP Server - Displays client info and exchanges hello messages
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF 1024

int main() {
    int sock, client;
    socklen_t len;
    struct sockaddr_in server, caddr;
    char buf[BUF];

    // socket() creates TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configure server to listen on all interfaces
    server.sin_family = AF_INET;
    server.sin_port = htons(3388);  // htons() converts port to network byte order
    server.sin_addr.s_addr = INADDR_ANY;  // INADDR_ANY = accept on any network interface (0.0.0.0)
    
    // bind() associates socket with port
    bind(sock, (struct sockaddr*)&server, sizeof(server));
    
    // listen() marks socket as passive (ready to accept connections), backlog=1
    listen(sock, 1);
    printf("Listening on port 3388...\n");

    // accept() blocks until client connects, returns new socket for that connection
    // Client address info is stored in 'caddr'
    len = sizeof(caddr);
    client = accept(sock, (struct sockaddr*)&caddr, &len);

    // Display client socket information
    printf("\nClient Info:\n");
    printf("Address: %p\n", (void*)&caddr);  // %p prints memory address
    printf("IP: %s\n", inet_ntoa(caddr.sin_addr));  // inet_ntoa() converts binary IP to string
    printf("Port: %d\n\n", ntohs(caddr.sin_port));  // ntohs() converts network to host byte order

    // Receive message from client
    memset(buf, 0, BUF);  // memset() fills buffer with 0s
    recv(client, buf, BUF, 0);  // recv() receives data from connected socket
    printf("Client message: %s\n", buf);

    // Send greeting response
    strcpy(buf, "Hello from server!");  // strcpy() copies string to buffer
    send(client, buf, strlen(buf), 0);  // send() transmits data

    close(client);  // close() closes client connection
    close(sock);    // close() closes listening socket
    return 0;
}
