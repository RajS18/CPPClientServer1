// Client.cpp
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize WinSock." << std::endl;
        return 1;
    }

    SOCKET clientSocket;
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //localhost for now

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    char message[] = "Hello, Server Boy. WHat is up!";
    if (send(clientSocket, message, sizeof(message), 0) < 0) {
        std::cerr << "Failed to send message to server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    char buffer[1024];
    int bytesRead;

    if ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        std::cout << "Received from server: " << buffer << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
