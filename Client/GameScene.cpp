#include "GameScene.h"
#include "SceneManager.h"

extern SceneManager* sceneManager;

void GameScene::Init() {}

void GameScene::Update() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        sceneManager->ChangeScene(MENU);
    }
}

void GameScene::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Jeu en cours - Appuyez sur ESC pour retourner au menu", 50, 50, 20, WHITE);
    EndDrawing();
}

void GameScene::Unload() {}