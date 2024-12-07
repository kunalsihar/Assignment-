
#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9600
#define SERVER_IP "127.0.0.1"
#define OUTPUT_FILE "config/received_config.bin"

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024] = "abcdefg"; // Secret key

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    ssize_t n = sendto(sockfd, buffer, strlen(buffer), 0, 
                       (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0) {
        perror("Error sending request");
        close(sockfd);
        return 1;
    }

    std::ofstream file(OUTPUT_FILE, std::ios::binary);
    if (!file) {
        perror("Error opening output file");
        close(sockfd);
        return 1;
    }

    socklen_t len = sizeof(server_addr);
    while ((n = recvfrom(sockfd, buffer, sizeof(buffer), 0, 
                         (struct sockaddr*)&server_addr, &len)) > 0) {
        file.write(buffer, n);
    }

    if (n < 0) {
        perror("Error receiving file");
    } else {
        std::cout << "File received and saved successfully." << std::endl;
    }

    file.close();
    close(sockfd);
    return 0;
}
