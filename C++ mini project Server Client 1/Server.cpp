// Server.cpp
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize WinSock." << std::endl;
        return 1;
    }

    SOCKET serverSocket;
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is running and waiting for Miss Client . . . . " << std::endl;

    SOCKET clientSocket;
    sockaddr_in clientAddress;
    int clientAddressSize = sizeof(clientAddress);

    if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize)) == INVALID_SOCKET) {
        std::cerr << "Failed to accept client connection." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    char buffer[1024];
    int bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        std::cout << "Received: " << buffer << std::endl;
        send(clientSocket, buffer, bytesRead, 0);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
