#pragma once

#include <string>

#include "GameState.h"
class NetworkManager {
public:
    static NetworkManager& GetInstance();

    bool Initialize();
    void Shutdown();

    bool ConnectToServer(const std::string& serverIP, int playerID, const std::string& username);
    bool SendData(int matchID, int playerID);
    bool ReceiveData();

private:
    NetworkManager();
    ~NetworkManager();
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    void* clientSocket;  // On utilise `void*` pour éviter Winsock ici
    void* serverAddr;
    SimpleGameState gameState;
};