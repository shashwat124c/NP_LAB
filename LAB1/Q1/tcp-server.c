#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 3388
#define BUF 1024

int asc(const void *a, const void *b) { 
    return *(int*)a - *(int*)b;  // Cast void* to int* then dereference
}

int desc(const void *a, const void *b) { 
    return *(int*)b - *(int*)a; 
}

// Convert str into int array
int parse(char *s, int *arr) {
    int n = 0;
    // strtok() splits string by delimiter, returns tokens one by one
    for (char *t = strtok(s, " "); t; t = strtok(NULL, " "))
        arr[n++] = atoi(t);  
    return n;
}

int main() {
    int sock, client, n, arr[100];
    socklen_t len;  
    struct sockaddr_in server, caddr;
    char buf[BUF];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);  
    server.sin_addr.s_addr = INADDR_ANY;  
    
    bind(sock, (struct sockaddr*)&server, sizeof(server));
    
    listen(sock, 1);
    printf("Listening on port %d...\n", PORT);

    len = sizeof(caddr);
    client = accept(sock, (struct sockaddr*)&caddr, &len);
    printf("Client connected.\n");

    while (1) {
        memset(buf, 0, BUF);  
        recv(client, buf, BUF, 0);  
        char op = buf[0];

        if (op == '5') break;

        memset(buf, 0, BUF); 
        recv(client, buf, BUF, 0); 
        n = atoi(buf); 
        
        memset(buf, 0, BUF); 
        recv(client, buf, BUF, 0); 
        parse(buf, arr);

        if (op == '1') {
            memset(buf, 0, BUF); 
            recv(client, buf, BUF, 0);
            int target = atoi(buf), pos = -1;
            
            // Linear search through array
            for (int i = 0; i < n; i++)
                if (arr[i] == target) { 
                    pos = i + 1;  
                    break; 
                }
            sprintf(buf, pos > 0 ? "Found at position %d" : "Not found", pos);
        }
        else if (op == '2' || op == '3') {
            // qsort(array, count, element_size, comparison_function)
            qsort(arr, n, sizeof(int), op == '2' ? asc : desc);
            
            // Build result string
            sprintf(buf, "Sorted: ");
            for (int i = 0; i < n; i++) 
                sprintf(buf + strlen(buf), "%d ", arr[i]);  // Append to existing string
        }
        else if (op == '4') {
            // Split into even and odd numbers
            sprintf(buf, "Even: ");
            for (int i = 0; i < n; i++) 
                if (arr[i] % 2 == 0)  // % is modulo operator (remainder after division)
                    sprintf(buf + strlen(buf), "%d ", arr[i]);
            
            strcat(buf, "\nOdd: ");  // strcat() concatenates strings
            for (int i = 0; i < n; i++) 
                if (arr[i] % 2 != 0)
                    sprintf(buf + strlen(buf), "%d ", arr[i]);
        }
        
        // send() transmits data to connected client
        send(client, buf, strlen(buf), 0);
    }
    
    // close() closes sockets and releases resources
    close(client);
    close(sock);
    return 0;
}
