#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <thread>  

#pragma comment(lib, "ws2_32.lib") // Lien avec la bibliothèque réseau de Windows

#define SERVER_IP "127.0.0.1"  // Adresse IP du serveur (localhost)
#define PORT 8080              // Port utilisé pour la connexion

// Structure pour envoyer les entrées du joueur au serveur
struct PlayerInput {
    int matchID;  // Identifiant du match
    int playerID; // Identifiant du joueur (1 ou 2)
    bool moveUp;  // Indique si le joueur veut monter
    bool moveDown; // Indique si le joueur veut descendre
};

// Structure pour recevoir l'état du jeu depuis le serveur
struct SimpleGameState {
    int frameID;    // Identifiant de l'image (utile pour synchroniser)
    float player1Y; // Position Y du joueur 1
    float player2Y; // Position Y du joueur 2
    int score1;     // Score du joueur 1
    int score2;     // Score du joueur 2
    float ballx;    // Position X de la balle
    float bally;    // Position Y de la balle
};

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    int lastFrameID = -1;

    // Initialisation de la bibliothèque réseau WinSock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Création d'un socket UDP
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    // Configuration de l'adresse du serveur
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // Création d'une adresse locale pour le client
    sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = htons(0); // Choix automatique du port

    // Liaison du socket à l'adresse locale
    if (bind(clientSocket, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
        std::cerr << "[CLIENT] Erreur de bind sur un port local\n";
        return 1;
    }

    // Demande à l'utilisateur d'entrer un match ID et un player ID
    int matchID, playerID;
    std::cout << "Entrez un Match ID : ";
    std::cin >> matchID;
    std::cout << "Entrez un Player ID (1 ou 2) : ";
    std::cin >> playerID;

    // Boucle principale du jeu
    while (true) {
        // Création de l'objet qui contient les entrées du joueur
        PlayerInput input = { matchID, playerID, false, false };

        // Vérification des touches pressées pour le déplacement
        if (playerID == 1) {
            if (GetAsyncKeyState('Z') & 0x8000) input.moveUp = true;
            if (GetAsyncKeyState('S') & 0x8000) input.moveDown = true;
        }
        else if (playerID == 2) {
            if (GetAsyncKeyState('E') & 0x8000) input.moveUp = true;
            if (GetAsyncKeyState('D') & 0x8000) input.moveDown = true;
        }

        // Envoi des entrées du joueur au serveur ( simule la connection avec le serveur)
        sendto(clientSocket, (char*)&input, sizeof(PlayerInput), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

        // Réception de l'état du jeu depuis le serveur ( lancer la partie )
        SimpleGameState gameState;
        int serverAddrSize = sizeof(serverAddr);
        int bytesReceived = recvfrom(clientSocket, (char*)&gameState, sizeof(SimpleGameState), 0, (sockaddr*)&serverAddr, &serverAddrSize);

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "[CLIENT] Erreur de réception: " << WSAGetLastError() << std::endl;
        }
        else if (bytesReceived == sizeof(SimpleGameState)) {
            // Vérifie si l'adversaire s'est déconnecté
            if (gameState.frameID == -1) {
                std::cout << "[CLIENT] L'adversaire s'est déconnecté. Fin du match.\n";
                break;
            }

            // Affichage des informations du jeu
            std::cout << "[CLIENT] Frame " << gameState.frameID
                << " | Ball (X: " << gameState.ballx << ", Y: " << gameState.bally << ")"
                << " | Joueur 1 Y: " << gameState.player1Y
                << " | Joueur 2 Y: " << gameState.player2Y
                << " | Score: " << gameState.score1 << " - " << gameState.score2
                << std::endl;
        }
    }

    // Fermeture du socket et nettoyage de WinSock
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
