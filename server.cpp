#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

// Define the port number and buffer size
const int PORT = 8080;
const int BUFFER_SIZE = 1024;

// Dummy function to simulate sending data to an API
void sendDataToAPI(const char* data) {
    // Replace this with actual API communication code
    std::cout << "Sending data to API: " << data << std::endl;
}

// Function to handle a client connection
void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    int bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the received data
        std::cout << "Received from client: " << buffer << std::endl;

        // Simulate sending data to API
        sendDataToAPI(buffer);

        // Echo the data back to the client
        send(clientSocket, buffer, strlen(buffer), 0);
    }

    // Close the client socket when done
    close(clientSocket);
}

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return EXIT_FAILURE;
    }

    // Configure the server address struct
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(serverSocket);
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(serverSocket);
        return EXIT_FAILURE;
    }

    std::cout << "Server listening on port " << PORT << "..." << std::endl;

    while (true) {
        // Accept a client connection
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        // Create a new thread to handle the client
        std::thread(handleClient, clientSocket).detach();
    }

    // Close the server socket when done (will never reach this point in this example)
    close(serverSocket);

    return 0;
}
