#include "NetworkManager.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

NetworkManager::NetworkManager() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[NetworkManager] Erreur WSAStartup" << std::endl;
    }

    // Création du socket UDP
    clientSocket = (void*)socket(AF_INET, SOCK_DGRAM, 0);
    if ((SOCKET)clientSocket == INVALID_SOCKET) {
        std::cerr << "[NetworkManager] Erreur de création du socket" << std::endl;
    }

    serverAddr = new sockaddr_in();
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


bool NetworkManager::SendData(const std::string& data) {
    sockaddr_in* addr = (sockaddr_in*)serverAddr;
    if (sendto((SOCKET)clientSocket, data.c_str(), data.length(), 0, (sockaddr*)addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        std::cerr << "[NetworkManager] Erreur d'envoi des données" << std::endl;
        return false;
    }
    return true;
}

std::string NetworkManager::ReceiveData() {
    char buffer[256];
    sockaddr_in* addr = (sockaddr_in*)serverAddr;
    int addrSize = sizeof(sockaddr_in);
    int bytesReceived = recvfrom((SOCKET)clientSocket, buffer, sizeof(buffer), 0, (sockaddr*)addr, &addrSize);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        return std::string(buffer);
    }
    return "";
}

bool NetworkManager::IsGameReady() {
    std::cout << "[NetworkManager] Vérification du nombre de joueurs connectés..." << std::endl;

    // ?? Envoyer une requête pour savoir combien de joueurs sont connectés
    std::string request = "PLAYERS";
    if (!SendData(request)) return false;

    // ?? Recevoir la réponse du serveur
    std::string response = ReceiveData();

    if (response == "2") {
        std::cout << "[NetworkManager] 2 joueurs connectés, la partie peut commencer !" << std::endl;
        return true;
    }

    std::cout << "[NetworkManager] En attente d'un autre joueur..." << std::endl;
    return false;
}