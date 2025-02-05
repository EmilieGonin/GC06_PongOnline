#pragma once

#define BALL_SPEED 4.0f

class GameData
{
public:
    virtual ~GameData() = default;
};

struct PlayerInput : public GameData
{
    int matchID;
    int playerID;
    bool moveUp;
    bool moveDown;

    PlayerInput(int matchID = 0, int playerID = 0, bool moveUp = false, bool moveDown = false)
        : matchID(matchID), playerID(playerID), moveUp(moveUp), moveDown(moveDown) {}
};

struct GameState : public GameData
{
    float ballX, ballY;
    float ballVelX, ballVelY;
    float player1Y, player2Y;
    int score1, score2;

    GameState(float ballX = 400.0f, float ballY = 300.0f, float ballVelX = BALL_SPEED, float ballVelY = BALL_SPEED,
        float player1Y = 250.0f, float player2Y = 250.0f, int score1 = 0, int score2 = 0)
        : ballX(ballX), ballY(ballY), ballVelX(ballVelX), ballVelY(ballVelY),
        player1Y(player1Y), player2Y(player2Y), score1(score1), score2(score2) {}
};

struct SimpleGameState : public GameData
{
    int frameID;
    float player1Y;
    float player2Y;
    int score1;
    int score2;
    float ballx;
    float bally;

    SimpleGameState(int frameID = 0, float player1Y = 0, float player2Y = 0,
        int score1 = 0, int score2 = 0, float ballx = 0, float bally = 0)
        : frameID(frameID), player1Y(player1Y), player2Y(player2Y),
        score1(score1), score2(score2), ballx(ballx), bally(bally) {}
};