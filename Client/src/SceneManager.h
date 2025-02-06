#pragma once

#include "Scene.h"
#include <string>

enum SceneType { MENU, GAME };

class SceneManager {
public:
    static SceneManager& GetInstance();

    void ChangeScene(SceneType type);
    void Update();
    void Draw();

    // Stockage des infos de connexion
    void SetUsername(const std::string& name) { username = name; }
    void SetServerIP(const std::string& ip) { serverIP = ip; }
    void SetPlayerID(int id) { playerID = id; }

    std::string GetUsername() const { return username; }
    std::string GetServerIP() const { return serverIP; }
    int GetPlayerID() const { return playerID; }

private:
    SceneManager();
    ~SceneManager();
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    Scene* currentScene;

    std::string username = "Joueur";
    std::string serverIP = "127.0.0.1";
    int playerID = 1;  // Par défaut, joueur 1
};
