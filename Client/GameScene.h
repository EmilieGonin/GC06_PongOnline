#pragma once

#include "Scene.h"
#include "raylib.h"

class GameScene : public Scene {
public:
    void Init() override;
    void Update() override;
    void Draw() override;
    void Unload() override;
};

