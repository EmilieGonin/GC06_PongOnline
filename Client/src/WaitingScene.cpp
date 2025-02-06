#include "WaitingScene.h"

WaitingScene::WaitingScene() {}

WaitingScene::~WaitingScene() {}

void WaitingScene::Init() {}

void WaitingScene::Update() {
    SceneManager& sceneManager = SceneManager::GetInstance();

    // Vérifier si 2 joueurs sont prêts
    if (NetworkManager::GetInstance().IsGameReady()) {
        sceneManager.ChangeScene(GAME);
    }
}

void WaitingScene::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("En attente d'un autre joueur...", 100, 300, 30, DARKGRAY);

    EndDrawing();
}

void WaitingScene::Unload() {}