#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <openssl/sha.h>

std::string sha256(const std::string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.length());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error in socket creation" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Port number
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error in connection" << std::endl;
        return 1;
    }

    std::cout << "Connected to server" << std::endl;

    // Prompt user for activation key
    std::cout << "Enter activation key: ";
    std::string activationKey;
    std::cin >> activationKey;

    // Hash the activation key
    std::string hashedKey = sha256(activationKey);

    // Send hashed activation key to server
    send(clientSocket, hashedKey.c_str(), hashedKey.length(), 0);

    // Receive response from server
    recv(clientSocket, buffer, 1024, 0);
    std::cout << "Server response: " << buffer << std::endl;

    // Close the socket
    close(clientSocket);

    return 0;
}
