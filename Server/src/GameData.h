#pragma once
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

    PlayerInput(int matchID, int playerID, bool moveUp, bool moveDown)
        : matchID(matchID), playerID(playerID), moveUp(moveUp), moveDown(moveDown) {}
};

struct GameState : public GameData
{
    float ballX, ballY;
    float ballVelX, ballVelY;
    float player1Y, player2Y;
    int score1, score2;

    GameState(float ballX, float ballY, float ballVelX, float ballVelY, float player1Y, float player2Y, int score1, int score2)
        : ballX(ballX), ballY(ballY), ballVelX(ballVelX), ballVelY(ballVelY), player1Y(player1Y), player2Y(player2Y), score1(score1), score2(score2) {}
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

    SimpleGameState(int frameID, float player1Y, float player2Y, int score1, int score2, float ballx, float bally)
        : frameID(frameID), player1Y(player1Y), player2Y(player2Y), score1(score1), score2(score2), ballx(ballx), bally(bally) {}
};