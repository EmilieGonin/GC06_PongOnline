#include "GameScene.h"
#include <iostream>

GameScene::GameScene() {
    SceneManager& sceneManager = SceneManager::GetInstance();

    std::string username = sceneManager.GetUsername();
    std::string serverIP = sceneManager.GetServerIP();
    int playerID = sceneManager.GetPlayerID();

    std::cout << "Démarrage du jeu pour " << username << " (Player " << playerID
        << ") sur serveur " << serverIP << std::endl;

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
    SetTargetFPS(60);
}

void GameScene::Update() {
    inputManager.Update();

    player1->Update(inputManager.IsUser1Up(), inputManager.IsUser1Down());
    player2->Update(inputManager.IsUser2Up(), inputManager.IsUser2Down());
    ball->Update();

    if (inputManager.ShouldQuit()) {
        SceneManager::GetInstance().ChangeScene(MENU, "");
    }
}

void GameScene::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    player1->Draw();
    player2->Draw();
    ball->Draw();

    EndDrawing();
}

void GameScene::Unload() {
    // Libération des ressources si nécessaire
}