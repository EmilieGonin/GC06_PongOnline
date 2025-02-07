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
std::map<int, std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point>> lastActivity;
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
    char buffer[BUFFER_SIZE] = {};
    int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
    if (bytesReceived == SOCKET_ERROR) return false;

    memcpy(&input, buffer, sizeof(PlayerInput));

    // Vérifier si on enregistre un nouveau joueur
    if (playerPairs[input.matchID].first.sin_port == 0) {
        playerPairs[input.matchID].first = clientAddr;
        lastActivity[input.matchID].first = std::chrono::steady_clock::now();
    }
    else if (playerPairs[input.matchID].second.sin_port == 0) {
        playerPairs[input.matchID].second = clientAddr;
        lastActivity[input.matchID].second = std::chrono::steady_clock::now();
    }
    else {
        // Mise à jour de l'activité des joueurs
        if (clientAddr.sin_port == playerPairs[input.matchID].first.sin_port) {
            lastActivity[input.matchID].first = std::chrono::steady_clock::now();
        }
        else if (clientAddr.sin_port == playerPairs[input.matchID].second.sin_port) {
            lastActivity[input.matchID].second = std::chrono::steady_clock::now();
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

bool IsMatchReady(int matchID) {
    return playerPairs[matchID].first.sin_port != 0 &&
        playerPairs[matchID].second.sin_port != 0 &&
        lastActivity[matchID].first.time_since_epoch().count() != 0 &&
        lastActivity[matchID].second.time_since_epoch().count() != 0;
}

void SendTestMessage(int matchID) {
    if (!IsMatchReady(matchID)) {
        std::cout << "[SERVEUR] En attente d'un deuxième joueur pour le match " << matchID << "...\n";
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

    char ack[3] = "OK";
    sockaddr_in sender;
    int senderSize = sizeof(sender);

    auto sendWithAck = [&](sockaddr_in& clientAddr) {
        if (clientAddr.sin_port != 0) {
            sendto(serverSocket, (char*)&simpleState, sizeof(SimpleGameState), 0,
                (sockaddr*)&clientAddr, clientAddrSize);

            // Attendre un accusé de réception
            int retries = 3;
            while (retries--) {
                int bytesReceived = recvfrom(serverSocket, ack, sizeof(ack), 0,
                    (sockaddr*)&sender, &senderSize);
                if (bytesReceived > 0 && strcmp(ack, "OK") == 0) {
                    return;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            std::cerr << "[SERVEUR] Aucune réponse du client, tentative suivante..." << std::endl;
        }
        };

    sendWithAck(playerPairs[matchID].first);
    sendWithAck(playerPairs[matchID].second);
}


void GameLoop() {
    using namespace std::chrono;
    const int TIMEOUT_SECONDS = 5;
    const milliseconds FRAME_TIME(1000 / 60);  // 60 FPS

    while (running) {
        auto now = steady_clock::now();

        for (auto it = games.begin(); it != games.end();) {
            int matchID = it->first;
            auto& gameState = it->second;

            // Vérifier si le match est prêt à jouer (deux joueurs actifs)
            if (!IsMatchReady(matchID)) {
                ++it;
                continue;
            }

            // Vérifier si un joueur est inactif
            bool player1Active = now - lastActivity[matchID].first < seconds(TIMEOUT_SECONDS);
            bool player2Active = now - lastActivity[matchID].second < seconds(TIMEOUT_SECONDS);

            if (!player1Active || !player2Active) {
                std::cout << "[SERVEUR] Match " << matchID << " terminé - Un joueur s'est déconnecté.\n";

                // Informer le joueur restant
                SimpleGameState disconnectMessage = {};
                disconnectMessage.frameID = -1;  // Indiquer la fin du match
                if (player1Active) {
                    sendto(serverSocket, (char*)&disconnectMessage, sizeof(SimpleGameState), 0,
                        (sockaddr*)&playerPairs[matchID].first, clientAddrSize);
                }
                if (player2Active) {
                    sendto(serverSocket, (char*)&disconnectMessage, sizeof(SimpleGameState), 0,
                        (sockaddr*)&playerPairs[matchID].second, clientAddrSize);
                }

                // Supprimer la partie
                playerPairs.erase(matchID);
                lastActivity.erase(matchID);
                it = games.erase(it);
                continue;
            }

            // ✅ Mise à jour de la position de la balle
            gameState.ballX += gameState.ballVelX;
            gameState.ballY += gameState.ballVelY;

            // ✅ Rebond contre les murs haut et bas
            if (gameState.ballY <= 0 || gameState.ballY >= SCREEN_HEIGHT) {
                gameState.ballVelY = -gameState.ballVelY;
            }

            // ✅ Collision avec le paddle gauche (Joueur 1)
            if (gameState.ballX <= 50 &&
                gameState.ballY >= gameState.player1Y &&
                gameState.ballY <= gameState.player1Y + 100) {
                gameState.ballVelX = -gameState.ballVelX;
            }

            // ✅ Collision avec le paddle droit (Joueur 2)
            if (gameState.ballX >= SCREEN_WIDTH - 50 &&
                gameState.ballY >= gameState.player2Y &&
                gameState.ballY <= gameState.player2Y + 100) {
                gameState.ballVelX = -gameState.ballVelX;
            }

            // ✅ Si la balle sort à gauche, point pour le joueur 2
            if (gameState.ballX < 0) {
                gameState.score2++;
                gameState.ballX = SCREEN_WIDTH / 2;
                gameState.ballY = SCREEN_HEIGHT / 2;
                gameState.ballVelX = BALL_SPEED;
                gameState.ballVelY = BALL_SPEED;
            }

            // ✅ Si la balle sort à droite, point pour le joueur 1
            if (gameState.ballX > SCREEN_WIDTH) {
                gameState.score1++;
                gameState.ballX = SCREEN_WIDTH / 2;
                gameState.ballY = SCREEN_HEIGHT / 2;
                gameState.ballVelX = -BALL_SPEED;
                gameState.ballVelY = BALL_SPEED;
            }

            // ✅ Envoyer l'état du jeu aux clients
            SendTestMessage(matchID);

            ++it;
        }

        std::this_thread::sleep_for(FRAME_TIME);
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

