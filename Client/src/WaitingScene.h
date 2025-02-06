#pragma once

#include "Scene.h"
#include "raylib.h"
#include "NetworkManager.h"
#include "SceneManager.h"
#include <string>

class WaitingScene : public Scene {
public:
    WaitingScene(const std::string& message);
    ~WaitingScene();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Unload() override;

private:
    std::string waitingMessage;
};