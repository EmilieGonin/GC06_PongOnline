#include "GameScene.h"

GameScene::GameScene() {
    player1 = new Paddle(50, GetScreenHeight() / 2 - 50);
    player2 = new Paddle(GetScreenWidth() - 70, GetScreenHeight() / 2 - 50);
    ball = new Ball(GetScreenWidth() / 2 - 10, GetScreenHeight() / 2 - 10);
}

GameScene::~GameScene() {
    delete player1;
    delete player2;
    delete ball;
}

void GameScene::Init() {
    InitWindow(800, 600, "Pong Game");
    SetTargetFPS(60);
}

void GameScene::Update(InputManager& inputManager) {
    inputManager.Update();

    // Joueur 1 (flèches directionnelles)
    player1->Update(inputManager.IsUser1Up(), inputManager.IsUser1Down());

    // Joueur 2 (Z/S)
    player2->Update(inputManager.IsUser2Up(), inputManager.IsUser2Down());

    // Mise à jour de la balle
    ball->Update();

    // Gestion des collisions simples avec les paddles
    if ((ball->GetX() <= player1->GetX() + player1->GetWidth() &&
        ball->GetY() + ball->GetSize().second >= player1->GetY() &&
        ball->GetY() <= player1->GetY() + player1->GetHeight()) ||
        (ball->GetX() + ball->GetSize().first >= player2->GetX() &&
            ball->GetY() + ball->GetSize().second >= player2->GetY() &&
            ball->GetY() <= player2->GetY() + player2->GetHeight())) {
        ball->InvertDirectionX();
    }
}

void GameScene::Draw() const {
    BeginDrawing();
    ClearBackground(BLACK);

    player1->Draw();
    player2->Draw();
    ball->Draw();

    EndDrawing();
}
