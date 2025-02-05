#pragma once

#include "Paddle.h"
#include "Ball.h"
#include "InputManager.h"
#include <raylib.h>

class GameScene {
public:
    GameScene();
    ~GameScene();

    void Init();
    void Update(InputManager& inputManager);
    void Draw() const;

private:
    Paddle* player1;
    Paddle* player2;
    Ball* ball;
};