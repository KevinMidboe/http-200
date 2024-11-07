#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>  // For inet_ntop

int server_fd;

void handle_signal(int signal) {
    printf("\nReceived signal %d, shutting down server...\n", signal);
    fflush(stdout);  // Ensure output is flushed immediately
    if (server_fd >= 0) {
        close(server_fd);
    }
    exit(0);  // Graceful exit after closing the socket
}

int main() {
    struct sockaddr_in address;
    struct sockaddr_in client_address;
    int client_fd;
    int addrlen = sizeof(address);
    int port = 80;

    // Disable stdout buffering
    setbuf(stdout, NULL);  // Disables buffering, output is immediate

    // Setup signal handler for graceful shutdown
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return 1;
    }

    // Configure the server address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Start listening for incoming connections
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        return 1;
    }

    // Output immediately after listening starts
    printf("Server listening on port %i...\n", port);
    fflush(stdout);  // Ensure output is flushed immediately

    while (1) {
        // Accept client connections
        client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        // Get the client's IP address
        char client_ip[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip)) == NULL) {
            perror("inet_ntop failed");
        } else {
            printf("Connection from IP: %s\n", client_ip);  // Log the client IP
        }

        // Send a simple HTTP 200 OK response
        char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\nserver: http-200\r\n\r\n";
        send(client_fd, response, strlen(response), 0);
        close(client_fd);  // Close the client connection after sending the response
    }

    return 0;
}

