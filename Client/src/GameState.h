#pragma once

struct PlayerInput {
    int matchID;
    int playerID;
    bool moveUp;
    bool moveDown;
};

struct SimpleGameState {
    int frameID;
    int ballX;
    int ballY;
    int paddle1Y;
    int paddle2Y;
};