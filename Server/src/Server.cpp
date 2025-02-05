#include <iostream>
#include <winsock2.h>
#include <map>
#include <thread>
#include <chrono>
#include <atomic>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 8000
#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 800
#define PADDLE_SPEED 5.0f
#define BALL_SPEED 4.0f

struct GameState {
    float ballX, ballY;
    float ballVelX, ballVelY;
    float player1Y, player2Y;
    int score1, score2;
};

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

std::map<int, std::pair<sockaddr_in, sockaddr_in>> playerPairs;
std::map<int, GameState> games;
std::map<int, int> frameCounters;
std::atomic<bool> running(true);
SOCKET serverSocket;
sockaddr_in serverAddr, clientAddr;
int clientAddrSize = sizeof(clientAddr);
char buffer[BUFFER_SIZE];

bool InitializeServer();
bool ReceivePlayerInput(PlayerInput& input);
void UpdateGameState(const PlayerInput& input);
void SendTestMessage(int matchID);

bool InitializeServer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur WSAStartup\n";
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur de creation du socket\n";
        WSACleanup();
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur lors du bind du socket\n";
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    std::cout << " Serveur Pong demarre sur le port " << PORT << "...\n";
    return true;
}

bool ReceivePlayerInput(PlayerInput& input) {
    memset(buffer, 0, BUFFER_SIZE);
    int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0,
        (sockaddr*)&clientAddr, &clientAddrSize);

    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Erreur reception message. Code : " << WSAGetLastError() << "\n";
        return false;
    }

    memcpy(&input, buffer, sizeof(PlayerInput));

   
    if (playerPairs.find(input.matchID) == playerPairs.end()) {
        playerPairs[input.matchID].first = clientAddr;
        
    }
    else {
        
        if (playerPairs[input.matchID].first.sin_port == clientAddr.sin_port &&
            playerPairs[input.matchID].first.sin_addr.s_addr == clientAddr.sin_addr.s_addr) {
            std::cout << "[SERVEUR] Joueur 1 déjà enregistré avec cette IP.\n";
        }
        else {
            playerPairs[input.matchID].second = clientAddr;
          
        }
    }

    return true;
}


void UpdateGameState(const PlayerInput& input) {
    if (games.find(input.matchID) == games.end()) {
        games[input.matchID] = { 400, 300, BALL_SPEED, BALL_SPEED, 250, 250, 0, 0 };
    }

    GameState& gameState = games[input.matchID];

    
    if (input.playerID == 1) {
        if (input.moveUp && gameState.player1Y > 0) gameState.player1Y -= PADDLE_SPEED;
        if (input.moveDown && gameState.player1Y < SCREEN_HEIGHT - 100) gameState.player1Y += PADDLE_SPEED;
    }
    else if (input.playerID == 2) {
        if (input.moveUp && gameState.player2Y > 0) gameState.player2Y -= PADDLE_SPEED;
        if (input.moveDown && gameState.player2Y < SCREEN_HEIGHT - 100) gameState.player2Y += PADDLE_SPEED;
    }
}


void SendTestMessage(int matchID) {
    if (playerPairs.find(matchID) == playerPairs.end()) {
        return;
    }

    GameState& gameState = games[matchID];
    SimpleGameState simpleState;
    simpleState.frameID = ++frameCounters[matchID];
    simpleState.player1Y = gameState.player1Y;
    simpleState.player2Y = gameState.player2Y;
    simpleState.score1 = gameState.score1;
    simpleState.score2 = gameState.score2;
    simpleState.ballx = gameState.ballX;
    simpleState.bally = gameState.ballY;


    if (playerPairs[matchID].first.sin_port != 0) {
        std::cout << "[SERVEUR] Envoi au Joueur 1 (Port: " << ntohs(playerPairs[matchID].first.sin_port) << ")\n";
        sendto(serverSocket, (char*)&simpleState, sizeof(SimpleGameState), 0,
            (sockaddr*)&playerPairs[matchID].first, clientAddrSize);
    }

    if (playerPairs[matchID].second.sin_port != 0) {
        std::cout << "[SERVEUR] Envoi au Joueur 2 (Port: " << ntohs(playerPairs[matchID].second.sin_port) << ")\n";
        sendto(serverSocket, (char*)&simpleState, sizeof(SimpleGameState), 0,
            (sockaddr*)&playerPairs[matchID].second, clientAddrSize);
    }

    std::cout << "[SERVEUR] Match " << matchID << " - Frame " << simpleState.frameID
        << " | Ball (X: " << simpleState.ballx << ", Y: " << simpleState.bally << ")"
        << " | Joueur 1 Y: " << simpleState.player1Y
        << " | Joueur 2 Y: " << simpleState.player2Y
        << " | Score: " << simpleState.score1 << " - " << simpleState.score2
        << std::endl;
}


void GameLoop() {
    using namespace std::chrono;
    const int TICK_RATE = 60; 
    const milliseconds FRAME_TIME(1000 / TICK_RATE);

    while (true) {
        auto start = steady_clock::now();

        
        for (auto& [matchID, gameState] : games) {
            gameState.ballX += gameState.ballVelX;
            gameState.ballY += gameState.ballVelY;

           
            if (gameState.ballY <= 0 || gameState.ballY >= SCREEN_HEIGHT) {
                gameState.ballVelY = -gameState.ballVelY;
            }

            //  collisions avec les paddles
            if ((gameState.ballX <= 50 && gameState.ballY >= gameState.player1Y && gameState.ballY <= gameState.player1Y + 100) ||
                (gameState.ballX >= 750 && gameState.ballY >= gameState.player2Y && gameState.ballY <= gameState.player2Y + 100)) {
                gameState.ballVelX = -gameState.ballVelX;
            }

            
            if (gameState.ballX < 0) {
                gameState.score2++;
                gameState.ballX = 400; gameState.ballY = 300;
            }
            else if (gameState.ballX > SCREEN_WIDTH) {
                gameState.score1++;
                gameState.ballX = 400; gameState.ballY = 300;
            }

            
            SendTestMessage(matchID);
        }

        // Attendre la fin du tick
        auto end = steady_clock::now();
        std::this_thread::sleep_for(FRAME_TIME - duration_cast<milliseconds>(end - start));
    }
}



void StopServer() {
    running = false;
    closesocket(serverSocket);
    WSACleanup();
}

int main() {
    if (!InitializeServer()) {
        return EXIT_FAILURE;
    }

    std::thread gameThread(GameLoop); 

    while (running) {
        PlayerInput input;
        if (ReceivePlayerInput(input)) {
            UpdateGameState(input);
        }
    }

   
    if (gameThread.joinable()) {
        gameThread.join();
    }

    return 0;
}

