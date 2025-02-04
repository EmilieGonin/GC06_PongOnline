#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include<winsock2.h>
#define PORT 8080
#define BUFFER_SIZE 1024
#define SCREEN_HEIGHT 600
#define PADDLE_SPEED 250.0f
#define BALL_SPEED 150.0f
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
std::map<int, std::pair<sockaddr_in, sockaddr_in>> playerPairs;  
std::map<int, GameState> games;  

int main()
{
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    std::cout << "Serveur Pong en attente des clients...\n";
    int matchID = 0;
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0,
            (sockaddr*)&clientAddr, &clientAddrSize);

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Erreur réception\n";
            continue;
        }

        PlayerInput input;
        memcpy(&input, buffer, sizeof(PlayerInput));

        if (playerPairs.find(input.matchID) == playerPairs.end()) {
            playerPairs[input.matchID].first = clientAddr;
        }
        else {
            playerPairs[input.matchID].second = clientAddr;
        }

       
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

        
        gameState.ballX += gameState.ballVelX;
        gameState.ballY += gameState.ballVelY;

       
        if (gameState.ballY <= 0 || gameState.ballY >= SCREEN_HEIGHT) {
            gameState.ballVelY = -gameState.ballVelY;
        }

        
        if ((gameState.ballX <= 50 && gameState.ballY >= gameState.player1Y && gameState.ballY <= gameState.player1Y + 100) ||
            (gameState.ballX >= 750 && gameState.ballY >= gameState.player2Y && gameState.ballY <= gameState.player2Y + 100)) {
            gameState.ballVelX = -gameState.ballVelX;
        }

        if (gameState.ballX < 0) {
            gameState.score2++;
            gameState.ballX = 400; gameState.ballY = 300;
        }
        else if (gameState.ballX > 800) {
            gameState.score1++;
            gameState.ballX = 400; gameState.ballY = 300;
        }

        
        sendto(serverSocket, (char*)&gameState, sizeof(GameState), 0,
            (sockaddr*)&playerPairs[input.matchID].first, clientAddrSize);
        sendto(serverSocket, (char*)&gameState, sizeof(GameState), 0,
            (sockaddr*)&playerPairs[input.matchID].second, clientAddrSize);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;

}