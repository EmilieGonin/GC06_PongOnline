#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <thread>
#include <mutex>  // Ajout du mutex
#include <atomic>
#include "App.h"
#include "InputManager.h"
#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 8080


std::atomic<bool> running(true);
std::mutex gameMutex;  

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
} gameState;  

void Draw() {
    while (running) {
        {
            std::lock_guard<std::mutex> lock(gameMutex);  
            
           /* std::cout << "[DRAW] Ball (X: " << gameState.ballx << ", Y: " << gameState.bally << ")"
                << " | J1 Y: " << gameState.player1Y
                << " | J2 Y: " << gameState.player2Y
                << " | Score: " << gameState.score1 << " - " << gameState.score2
                << std::endl;*/
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));  
    }
}

void reseau() {
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

    int matchID, playerID;
    std::cout << "Entrez un Match ID : ";
    std::cin >> matchID;
    std::cout << "Entrez un Player ID (1 ou 2) : ";
    std::cin >> playerID;

    while (running) {
        PlayerInput input = { matchID, playerID, false, false };

        if (playerID == 1) {
            if (GetAsyncKeyState('Z') & 0x8000) input.moveUp = true;
            if (GetAsyncKeyState('S') & 0x8000) input.moveDown = true;
        }
        else if (playerID == 2) {
            if (GetAsyncKeyState('E') & 0x8000) input.moveUp = true;
            if (GetAsyncKeyState('D') & 0x8000) input.moveDown = true;
        }

        
        sendto(clientSocket, (char*)&input, sizeof(PlayerInput), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

        
        SimpleGameState newGameState;
        int serverAddrSize = sizeof(serverAddr);
        int bytesReceived = recvfrom(clientSocket, (char*)&newGameState, sizeof(SimpleGameState), 0, (sockaddr*)&serverAddr, &serverAddrSize);

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "[CLIENT] Erreur de réception: " << WSAGetLastError() << std::endl;
        }
        else if (bytesReceived == sizeof(SimpleGameState)) {
            if (newGameState.frameID == -1) {
                std::cout << "[CLIENT] L'adversaire s'est déconnecté. Fin du match.\n";
                running = false;
                break;
            }

           
            {
                std::lock_guard<std::mutex> lock(gameMutex);
                gameState = newGameState;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16)); 
    }

    closesocket(clientSocket);
    WSACleanup();
}

static void CheckInput(InputManager& im)
{
    while (running)
    {
        im.Update();

        int playerId = 1; // temp - get current player id

        if (im.ShouldQuit())
        {
            // quit
        }

        bool gameIsUp = true; // temp - check if game is currently playing

        if (gameIsUp)
        {
            if (playerId == 1 ? im.IsUser1Up() : im.IsUser2Up())
            {
                // request up
            }
            else if (playerId == 1 ? im.IsUser1Down() : im.IsUser2Down())
            {
                // request down
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

int main() {
    //InputManager im;

    std::thread gameThread(Draw);
    std::thread networkThread(reseau);
    //std::thread inputThread(CheckInput, std::ref(im));

    App app;
    app.Run();  

    //running = false;

    if (gameThread.joinable()) gameThread.join();
    if (networkThread.joinable()) networkThread.join();
    //if (inputThread.joinable()) inputThread.join();

    return 0;
}