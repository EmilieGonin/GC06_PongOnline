#include <iostream>
#include <winsock2.h>
#include <map>


#define PORT 8080
#define BUFFER_SIZE 1024
#define SCREEN_HEIGHT 600
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


std::map<int, std::pair<sockaddr_in, sockaddr_in>> playerPairs;
std::map<int, GameState> games;
SOCKET serverSocket;
sockaddr_in serverAddr, clientAddr;
int clientAddrSize = sizeof(clientAddr);
char buffer[BUFFER_SIZE];


bool InitializeServer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur WSAStartup\n";
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur de création du socket\n";
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

    std::cout << "Serveur Pong démarré sur le port " << PORT << "...\n";
    return true;
}


bool ReceivePlayerInput(PlayerInput& input) {
    memset(buffer, 0, BUFFER_SIZE);
    int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0,
        (sockaddr*)&clientAddr, &clientAddrSize);

    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Erreur lors de la réception du message\n";
        return false;
    }

    memcpy(&input, buffer, sizeof(PlayerInput));

   
    if (playerPairs.find(input.matchID) == playerPairs.end()) {
        playerPairs[input.matchID].first = clientAddr;
    }
    else {
        playerPairs[input.matchID].second = clientAddr;
    }

    return true;
}


void UpdateGameState(const PlayerInput& input) {
    if (games.find(input.matchID) == games.end()) {
        games[input.matchID] = { 400, 300, BALL_SPEED, BALL_SPEED, 250, 250, 0, 0 };
    }

    GameState& gameState = games[input.matchID];

    
    if (playerPairs[input.matchID].first.sin_port == 0 ||
        playerPairs[input.matchID].second.sin_port == 0) {
        std::cerr << "Attente d'un deuxième joueur pour le match " << input.matchID << "\n";
        return;
    }

    
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
}

void SendGameState(int matchID) {
    if (playerPairs.find(matchID) == playerPairs.end()) {
        std::cerr << "Match " << matchID << " introuvable !\n";
        return;
    }

    GameState& gameState = games[matchID];

    int sendResult1 = sendto(serverSocket, (char*)&gameState, sizeof(GameState), 0,
        (sockaddr*)&playerPairs[matchID].first, clientAddrSize);
    int sendResult2 = sendto(serverSocket, (char*)&gameState, sizeof(GameState), 0,
        (sockaddr*)&playerPairs[matchID].second, clientAddrSize);

    if (sendResult1 == SOCKET_ERROR || sendResult2 == SOCKET_ERROR) {
        std::cerr << "Erreur lors de l'envoi des données du match " << matchID << "\n";
    }
}


int main() {

    if (!InitializeServer()) {
        std::cerr << "Échec de l'initialisation du serveur. Fermeture du programme.\n";
        return EXIT_FAILURE;
    }
    while (true) {
        PlayerInput input;
        if (ReceivePlayerInput(input)) {
            UpdateGameState(input);
            SendGameState(input.matchID);
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
