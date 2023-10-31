#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

struct Packet {
    int header;
    char payload[256];
    int tail;
};

int main() {
    // Create a socket for the server to listen on
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Server address and port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the server socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Binding failed" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Listening failed" << std::endl;
        return 1;
    }

    // Accept incoming connections
    int clientSocket = accept(serverSocket, NULL, NULL);

    if (clientSocket < 0) {
        std::cerr << "Accepting connection failed" << std::endl;
        return 1;
    }

    // Connection accepted; receive the packet
    Packet receivedPacket;

    ssize_t bytesReceived = recv(clientSocket, &receivedPacket, sizeof(receivedPacket), 0);

    if (bytesReceived == sizeof(receivedPacket)) {
        std::cout << "Message received" << std::endl;

        // Print the received message
        std::cout << "Header: " << receivedPacket.header << std::endl;
        std::cout << "Payload: " << receivedPacket.payload << std::endl;
        std::cout << "Tail: " << receivedPacket.tail << std::endl;
    } else {
        std::cerr << "Failed to receive the entire packet from the client." << std::endl;
    }

    // Close the client and server sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
