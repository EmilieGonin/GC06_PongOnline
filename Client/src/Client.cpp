#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 8080

struct GameState {
    float ballX, ballY;
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
    float player1Y;
    float player2Y;
    int score1;
    int score2;
    float ballx;
    float bally;
};

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[sizeof(SimpleGameState)];

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    int matchID, playerID;
    std::cout << "Entrez un Match ID (ex: 1) : ";
    std::cin >> matchID;
    std::cout << "Entrez un Player ID (1 ou 2) : ";
    std::cin >> playerID;

    while (true) {
        PlayerInput input = { matchID, playerID, false, false };

       
        if (playerID == 1) {
            if (GetAsyncKeyState('Z') & 0x8000) input.moveUp = true;
            if (GetAsyncKeyState('S') & 0x8000) input.moveDown = true;
        }
        else if (playerID == 2) {
            if (GetAsyncKeyState('E') & 0x8000) input.moveUp = true;
            if (GetAsyncKeyState('D') & 0x8000) input.moveDown = true;
        }

        if (GetAsyncKeyState('Q') & 0x8000) break;

      
        sendto(clientSocket, (char*)&input, sizeof(PlayerInput), 0,
            (sockaddr*)&serverAddr, sizeof(serverAddr));

        
        SimpleGameState gameState;
        int serverAddrSize = sizeof(serverAddr);
        int bytesReceived = recvfrom(clientSocket, (char*)&gameState, sizeof(SimpleGameState), 0,
            (sockaddr*)&serverAddr, &serverAddrSize);

        if (bytesReceived == sizeof(SimpleGameState)) {
            std::cout << " Positions reçues : Joueur 1 Y = " << gameState.player1Y
                << " | Joueur 2 Y = " << gameState.player2Y << "|score 1 ="<<gameState.score1<<"| score 2 ="<<gameState.score2 <<
                "| ball y  = " << gameState.bally << "| ball x = " << gameState.ballx <<
                "\n";
        }
        else {
            std::cerr << " Erreur réception positions \n";
        }

        Sleep(100);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}