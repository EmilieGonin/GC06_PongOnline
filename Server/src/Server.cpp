#include <iostream> 
#include <winsock2.h>
#include <map>
#include <thread>
#include <chrono>
#include <atomic>
#pragma comment(lib, "ws2_32.lib") // Liaison avec la bibliothèque Winsock pour les sockets sous Windows

#define PORT 8080  // Port du serveur
#define BUFFER_SIZE 8000  // Taille du buffer pour la réception des données
#define SCREEN_HEIGHT 600  // Hauteur de l'écran
#define SCREEN_WIDTH 800  // Largeur de l'écran
#define PADDLE_SPEED 5.0f  // Vitesse des raquettes
#define BALL_SPEED 4.0f  // Vitesse de la balle

// Structure représentant l'état du jeu
struct GameState {
    float ballX, ballY;
    float ballVelX, ballVelY;
    float player1Y, player2Y;
    int score1, score2;
};

// Structure contenant l'entrée des joueurs
struct PlayerInput {
    int matchID;
    int playerID;
    bool moveUp;
    bool moveDown;
};

// Structure simplifiée pour envoyer l'état du jeu aux clients
struct SimpleGameState {
    int frameID;
    float player1Y;
    float player2Y;
    int score1;
    int score2;
    float ballx;
    float bally;
};

// Gestion des matchs et des connexions
std::map<int, std::pair<sockaddr_in, sockaddr_in>> playerPairs;
std::map<int, GameState> games;
std::map<int, int> frameCounters;
std::map<int, std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point>> lastActivity;

std::atomic<bool> running(true); // Variable pour stopper le serveur proprement
SOCKET serverSocket;
sockaddr_in serverAddr, clientAddr;
int clientAddrSize = sizeof(clientAddr);
char buffer[BUFFER_SIZE];

// Prototypes
bool InitializeServer();
bool ReceivePlayerInput(PlayerInput& input);
void UpdateGameState(const PlayerInput& input);
void SendTestMessage(int matchID);

// Initialisation du serveur
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

    std::cout << " Serveur Pong démarré sur le port " << PORT << "...\n";
    return true;
}

// Réception des entrées des joueurs
bool ReceivePlayerInput(PlayerInput& input) {
    char buffer[BUFFER_SIZE] = {};
    int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
    if (bytesReceived == SOCKET_ERROR) return false;

    memcpy(&input, buffer, sizeof(PlayerInput));

    // Attribution des joueurs au match
    if (playerPairs[input.matchID].first.sin_port == 0) {
        playerPairs[input.matchID].first = clientAddr;
        lastActivity[input.matchID].first = std::chrono::steady_clock::now();
    }
    else if (playerPairs[input.matchID].second.sin_port == 0) {
        playerPairs[input.matchID].second = clientAddr;
        lastActivity[input.matchID].second = std::chrono::steady_clock::now();
    }
    else {
        if (clientAddr.sin_port == playerPairs[input.matchID].first.sin_port) {
            lastActivity[input.matchID].first = std::chrono::steady_clock::now();
        }
        else if (clientAddr.sin_port == playerPairs[input.matchID].second.sin_port) {
            lastActivity[input.matchID].second = std::chrono::steady_clock::now();
        }
    }

    std::cout << "[SERVEUR] Entrée reçue - Match: " << input.matchID
        << ", Joueur: " << input.playerID
        << ", MoveUp: " << input.moveUp
        << ", MoveDown: " << input.moveDown << std::endl;

    return true;
}

// Mise à jour de l'état du jeu
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

    std::cout << "[SERVEUR] Mise à jour du match " << input.matchID
        << " | Joueur 1 Y: " << gameState.player1Y
        << " | Joueur 2 Y: " << gameState.player2Y << std::endl;
}
//verifier si les deux joueur sont co ou pas client 1 en attante 
bool IsMatchReady(int matchID) {
    return playerPairs[matchID].first.sin_port != 0 &&
        playerPairs[matchID].second.sin_port != 0 &&
        lastActivity[matchID].first.time_since_epoch().count() != 0 &&
        lastActivity[matchID].second.time_since_epoch().count() != 0;
}
// Envoi des données aux clients
void SendTestMessage(int matchID) {
    if (!IsMatchReady(matchID)) {
        std::cout << "[SERVEUR] En attente d'un deuxième joueur pour le match " << matchID << "...\n";
        return;
    }

    GameState& gameState = games[matchID];
    SimpleGameState simpleState = { ++frameCounters[matchID], gameState.player1Y, gameState.player2Y,
                                    gameState.score1, gameState.score2, gameState.ballX, gameState.ballY };

    sendto(serverSocket, (char*)&simpleState, sizeof(SimpleGameState), 0, (sockaddr*)&playerPairs[matchID].first, clientAddrSize);
    sendto(serverSocket, (char*)&simpleState, sizeof(SimpleGameState), 0, (sockaddr*)&playerPairs[matchID].second, clientAddrSize);

    std::cout << "[SERVEUR] Match " << matchID << " - Frame " << simpleState.frameID
        << " | Ball (X: " << simpleState.ballx << ", Y: " << simpleState.bally << ")"
        << " | Joueur 1 Y: " << simpleState.player1Y
        << " | Joueur 2 Y: " << simpleState.player2Y
        << " | Score: " << simpleState.score1 << " - " << simpleState.score2
        << std::endl;
}

// Boucle principale du jeu
void GameLoop() {
    using namespace std::chrono;
    const milliseconds FRAME_TIME(1000 / 60);

    while (running) {
        for (auto it = games.begin(); it != games.end();) {
            int matchID = it->first;
            auto& gameState = it->second;

            if (!IsMatchReady(matchID)) {
                ++it;
                continue;
            }

            gameState.ballX += gameState.ballVelX;
            gameState.ballY += gameState.ballVelY;

            SendTestMessage(matchID);
            ++it;
        }

        std::this_thread::sleep_for(FRAME_TIME);
    }
}

// Fonction pour arrêter proprement le serveur
void StopServer() {
    running = false;
    closesocket(serverSocket);
    WSACleanup();
}

// Fonction principale
int main() {
    if (!InitializeServer()) return EXIT_FAILURE;
    std::thread gameThread(GameLoop);

    while (running) {
        PlayerInput input;
        if (ReceivePlayerInput(input)) UpdateGameState(input);
    }

    if (gameThread.joinable()) gameThread.join();
    return 0;
}
