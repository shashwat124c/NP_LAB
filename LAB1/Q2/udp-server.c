#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>

#define BUF 1024

int is_palindrome(char *s) {
    int len = strlen(s);  
    for (int i = 0; i < len / 2; i++)
        if (s[i] != s[len - 1 - i])  
            return 0;
    return 1;
}

int main() {
    int sock;
    socklen_t len;
    struct sockaddr_in server, client;
    char buf[BUF], result[BUF];

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    server.sin_family = AF_INET;
    server.sin_port = htons(3388);  
    server.sin_addr.s_addr = INADDR_ANY;  
    
    bind(sock, (struct sockaddr*)&server, sizeof(server));
    printf("Listening on port 3388...\n");

    while (1) {
        len = sizeof(client);
        memset(buf, 0, BUF); 
        recvfrom(sock, buf, BUF, 0, (struct sockaddr*)&client, &len);

        if (strcmp(buf, "Halt") == 0) break;  

        int v[5] = {0};  // Initialize array to zeros: a, e, i, o, u
        for (int i = 0; buf[i]; i++) {  // Loop until null terminator
            char c = tolower(buf[i]);  // tolower() converts 'A'->'a', leaves 'a' as 'a'
            if (c == 'a') v[0]++;
            else if (c == 'e') v[1]++;
            else if (c == 'i') v[2]++;
            else if (c == 'o') v[3]++;
            else if (c == 'u') v[4]++;
        }

        sprintf(result, "%s palindrome\nLength: %lu\nVowels - A:%d E:%d I:%d O:%d U:%d",
                is_palindrome(buf) ? "Is" : "Not", strlen(buf), v[0], v[1], v[2], v[3], v[4]);

        sendto(sock, result, strlen(result), 0, (struct sockaddr*)&client, sizeof(client));
    }
    close(sock);
    return 0;
}
