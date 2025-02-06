#pragma once

#include "Scene.h"
#include "raylib.h"
#include "NetworkManager.h"
#include "SceneManager.h"

class WaitingScene : public Scene {
public:
    WaitingScene();
    ~WaitingScene();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Unload() override;
};