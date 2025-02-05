// GameScene.h
#pragma once

#include "Paddle.h"
#include "Ball.h"
#include "InputManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include <raylib.h>

class GameScene : public Scene {
public:
    GameScene();
    ~GameScene();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Unload() override;

private:
    Paddle* player1;
    Paddle* player2;
    Ball* ball;
    InputManager inputManager;
};