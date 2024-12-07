
#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9600
#define CONFIG_FILE "config/config.bin"

void handle_client(int sockfd) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];

    ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0, 
                         (struct sockaddr*)&client_addr, &client_len);
    if (n < 0) {
        perror("Error receiving data");
        return;
    }

    // Validate request (e.g., secret key)
    if (strcmp(buffer, "abcdefg") != 0) {
        std::cerr << "Invalid request received." << std::endl;
        return;
    }

    std::ifstream file(CONFIG_FILE, std::ios::binary);
    if (!file) {
        perror("Error opening config file");
        return;
    }

    // Send file
    while (file.read(buffer, sizeof(buffer))) {
        sendto(sockfd, buffer, file.gcount(), 0, 
               (struct sockaddr*)&client_addr, client_len);
    }
    // Send any remaining bytes
    if (file.gcount() > 0) {
        sendto(sockfd, buffer, file.gcount(), 0, 
               (struct sockaddr*)&client_addr, client_len);
    }
    std::cout << "Config file sent successfully." << std::endl;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    std::cout << "Server is running and waiting for requests..." << std::endl;
    while (true) {
        handle_client(sockfd);
    }

    close(sockfd);
    return 0;
}
