#include "NetworkManager.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 8080
std::atomic<bool> running(true);
std::mutex gameMutex;

NetworkManager::NetworkManager() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = htons(0);

    if (bind(clientSocket, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
        std::cerr << "[CLIENT] Erreur de bind sur un port local\n";
        running = false;
        return;
    }
}

NetworkManager::~NetworkManager() {
    Shutdown();
    delete (sockaddr_in*)serverAddr;
}

NetworkManager& NetworkManager::GetInstance() {
    static NetworkManager instance;
    return instance;
}

bool NetworkManager::Initialize() {
    return (SOCKET)clientSocket != INVALID_SOCKET;
}

void NetworkManager::Shutdown() {
    closesocket((SOCKET)clientSocket);
    WSACleanup();
}

bool NetworkManager::ConnectToServer(const std::string& serverIP, int playerID, const std::string& username) {
    std::cout << "[NetworkManager] Tentative de connexion à " << serverIP << " en tant que joueur " << playerID << std::endl;

    sockaddr_in* addr = (sockaddr_in*)serverAddr;
    addr->sin_family = AF_INET;
    addr->sin_port = htons(8080);
    inet_pton(AF_INET, serverIP.c_str(), &addr->sin_addr);

    // Construire le message de connexion
    std::string message = "CONNECT " + std::to_string(playerID) + " " + username;
    std::cout << "[NetworkManager] Envoi du message : " << message << std::endl;

    if (sendto((SOCKET)clientSocket, message.c_str(), message.length(), 0, (sockaddr*)addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        std::cerr << "[NetworkManager] Erreur lors de l'envoi de la connexion" << std::endl;
        return false;
    }

    // Attendre la réponse du serveur
    char response[30];
    int addrSize = sizeof(sockaddr_in);
    int bytesReceived = recvfrom((SOCKET)clientSocket, response, sizeof(response), 0, (sockaddr*)addr, &addrSize);

    if (bytesReceived > 0) {
        response[bytesReceived] = '\0';
        std::cout << "[NetworkManager] Réponse du serveur : " << response << std::endl;

        if (strcmp(response, "OK") == 0) {
            std::cout << "[NetworkManager] Connexion réussie !" << std::endl;
            return true;
        }
    }

    std::cerr << "[NetworkManager] Connexion échouée." << std::endl;
    return false;
}


void NetworkManager::SendData(int matchID, int playerID) {
    PlayerInput input = { matchID, playerID, false, false };

    if (playerID == 1) {
        if (GetAsyncKeyState('Z') & 0x8000) input.moveUp = true;
        if (GetAsyncKeyState('S') & 0x8000) input.moveDown = true;
    }
    else if (playerID == 2) {
        if (GetAsyncKeyState('E') & 0x8000) input.moveUp = true;
        if (GetAsyncKeyState('D') & 0x8000) input.moveDown = true;
    }


    sendto((SOCKET)clientSocket, (char*)&input, sizeof(PlayerInput), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
}

bool NetworkManager::ReceiveData() {
    SimpleGameState newGameState;
    int serverAddrSize = sizeof(serverAddr);
    int bytesReceived = recvfrom((SOCKET)clientSocket, (char*)&newGameState, sizeof(SimpleGameState), 0, (sockaddr*)&serverAddr, &serverAddrSize);

    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "[CLIENT] Erreur de réception: " << WSAGetLastError() << std::endl;
        return false; // Échec de réception
    }
    else if (bytesReceived == sizeof(SimpleGameState)) {
        if (newGameState.frameID == -1) {
            std::cout << "[CLIENT] L'adversaire s'est déconnecté. Fin du match.\n";
            running = false;
            return false; // Indique que la connexion a été perdue
        }

        {
            std::lock_guard<std::mutex> lock(gameMutex);
            gameState = newGameState;
        }
        return true; // Réception réussie
    }
    return false; // Retourne false si la réception n'est pas complète
}