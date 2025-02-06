#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <thread>  
#include "App.h"
#include <atomic>
#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 8080
std::atomic<bool> running(true);
struct PlayerInput {
    int matchID;
    int playerID;
    bool moveUp;
    bool moveDown;
};

struct SimpleGameState {
    int frameID;
    float player1Y;
    float player2Y;
    int score1;
    int score2;
    float ballx;
    float bally;
};
void Draw() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        //draw objects : paddels , ball , score
    }
}
int reseau() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    int lastFrameID = -1;

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
        return 1;
    }
    int matchID, playerID;
    std::cout << "Entrez un Match ID : ";
    std::cin >> matchID;
    std::cout << "Entrez un Player ID (1 ou 2) : ";
    std::cin >> playerID;

    while (true) {
        PlayerInput input = { matchID, playerID, false, false };

        // Gestion des touches
        if (playerID == 1) {
            if (GetAsyncKeyState('Z') & 0x8000) input.moveUp = true;
            if (GetAsyncKeyState('S') & 0x8000) input.moveDown = true;
        }
        else if (playerID == 2) {
            if (GetAsyncKeyState('E') & 0x8000) input.moveUp = true;
            if (GetAsyncKeyState('D') & 0x8000) input.moveDown = true;
        }

        // Envoi des inputs au serveur
        sendto(clientSocket, (char*)&input, sizeof(PlayerInput), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

        // Réception des données du serveur
        SimpleGameState gameState;
        int serverAddrSize = sizeof(serverAddr);
        int bytesReceived = recvfrom(clientSocket, (char*)&gameState, sizeof(SimpleGameState), 0, (sockaddr*)&serverAddr, &serverAddrSize);

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "[CLIENT] Erreur de réception: " << WSAGetLastError() << std::endl;
        }
        else if (bytesReceived == sizeof(SimpleGameState)) {
            // ✅ Vérifier si l'adversaire s'est déconnecté
            if (gameState.frameID == -1) {
                std::cout << "[CLIENT] L'adversaire s'est déconnecté. Fin du match.\n";
                break; // Quitter la boucle et terminer le client proprement
            }

            // Affichage des données du jeu
            std::cout << "[CLIENT] Frame " << gameState.frameID
                << " | Ball (X: " << gameState.ballx << ", Y: " << gameState.bally << ")"
                << " | Joueur 1 Y: " << gameState.player1Y
                << " | Joueur 2 Y: " << gameState.player2Y
                << " | Score: " << gameState.score1 << " - " << gameState.score2
                << std::endl;
        }
    }



    closesocket(clientSocket);
    WSACleanup();
    return 0;
}


int main() {
    std::thread gameThread(Draw);  

    App app;
    app.Run();  

    running = false;  

    if (gameThread.joinable()) {
        gameThread.join();  
    }

    return reseau();  
}