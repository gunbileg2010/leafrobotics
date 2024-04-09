#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
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
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);
    char buffer[1024];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error in socket creation" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Port number
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error in binding" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Error in listening" << std::endl;
        return 1;
    }

    std::cout << "Server listening..." << std::endl;

    // Accept a client connection
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
    if (clientSocket < 0) {
        std::cerr << "Error in accepting connection" << std::endl;
        return 1;
    }

    std::cout << "Connection established with client" << std::endl;

    // Receive hashed activation key from client
    recv(clientSocket, buffer, 1024, 0);
    std::string hashedKey(buffer);

    // Hash the expected activation key
    std::string expectedHash = sha256("valid_key");

    // Perform activation logic (compare hashed keys)
    std::string response;
    if (hashedKey == expectedHash) {
        response = "Activation successful";
    } else {
        response = "Invalid activation key";
    }

    // Send response to client
    send(clientSocket, response.c_str(), response.length(), 0);

    // Close the sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
