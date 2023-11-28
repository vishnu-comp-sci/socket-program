#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return EXIT_FAILURE;
    }

    // Configure the server address struct
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        std::cerr << "Invalid address/Address not supported" << std::endl;
        close(clientSocket);
        return EXIT_FAILURE;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Connection failed" << std::endl;
        close(clientSocket);
        return EXIT_FAILURE;
    }

    // Send a message to the server
    const char* message = "Hello, Server!";
    send(clientSocket, message, strlen(message), 0);
    std::cout << "Sent to server: " << message << std::endl;

    // Receive the response from the server
    char buffer[BUFFER_SIZE];
    int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytesRead == -1) {
        std::cerr << "Error receiving data from server" << std::endl;
    } else {
        buffer[bytesRead] = '\0'; // Null-terminate the received data
        std::cout << "Received from server: " << buffer << std::endl;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
