// TCP Client - Displays socket info and exchanges hello messages
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

    // socket() creates TCP socket (SOCK_STREAM = connection-oriented)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // Configure server address to connect to
    server.sin_family = AF_INET;
    server.sin_port = htons(3388);  // htons() converts port number to network byte order
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // inet_addr() converts IP string to binary
    
    // Configure and bind to specific client port
    client.sin_family = AF_INET;
    client.sin_port = htons(3389);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(sock, (struct sockaddr*)&client, sizeof(client));  // bind() attaches socket to address
    
    // connect() establishes TCP connection to server
    connect(sock, (struct sockaddr*)&server, sizeof(server));

    // getsockname() retrieves actual local address assigned to socket
    getsockname(sock, (struct sockaddr*)&client, &len);
    
    // Display client socket information
    printf("\nClient Info:\n");
    printf("Address: %p\n", (void*)&client);  // %p prints pointer address in hexadecimal
    printf("IP: %s\n", inet_ntoa(client.sin_addr));  // inet_ntoa() converts binary IP to "x.x.x.x"
    printf("Port: %d\n\n", ntohs(client.sin_port));  // ntohs() converts network to host byte order
    printf("Connected to Server.\n");

    // Send greeting message
    strcpy(buf, "Hello from client!");  // strcpy() copies string to buffer
    send(sock, buf, strlen(buf), 0);  // send() transmits data over TCP connection

    // Receive server response
    memset(buf, 0, BUF);  // memset() zeros out buffer before receiving
    recv(sock, buf, BUF, 0);  // recv() receives data from socket
    printf("\nServer Response: %s\n", buf);

    close(sock);  // close() releases socket resources
    return 0;
}
