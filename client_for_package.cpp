#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

struct Packet {
    int header;
    char payload[256];
    int tail;
};

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Server address and port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {
        // Create a dummy packet
        Packet dummyPacket;

        // Populate the packet
        dummyPacket.header = 123;
        std::string payloadData = "This is the payload data";
        std::strcpy(dummyPacket.payload, payloadData.c_str());
        dummyPacket.tail = 456;

        // Send the packet data
        ssize_t bytesSent = send(clientSocket, &dummyPacket, sizeof(dummyPacket), 0);

        if (bytesSent == sizeof(dummyPacket)) {
            std::cout << "Message sent" << std::endl;
        } else {
            std::cerr << "Failed to send the entire packet." << std::endl;
        }

        // Close the socket
        close(clientSocket);
    } else {
        std::cerr << "Failed to connect to the server." << std::endl;
    }

    return 0;
}