#pragma once

#include <string>

class NetworkManager {
public:
    static NetworkManager& GetInstance();

    bool Initialize();
    void Shutdown();

    bool ConnectToServer(const std::string& serverIP, int playerID, const std::string& username);
    bool SendData(const std::string& data);
    std::string ReceiveData();

    bool IsGameReady();

private:
    NetworkManager();
    ~NetworkManager();
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    void* clientSocket;  // On utilise `void*` pour éviter Winsock ici
    void* serverAddr;

};