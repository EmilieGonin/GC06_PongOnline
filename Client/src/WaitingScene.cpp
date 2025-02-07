#include "WaitingScene.h"

WaitingScene::WaitingScene(const std::string& message) : waitingMessage(message) {}

WaitingScene::~WaitingScene() {}

void WaitingScene::Init() {}

void WaitingScene::Update() {
    SceneManager& sceneManager = SceneManager::GetInstance();

    // V�rifier si 2 joueurs sont pr�ts
    if (NetworkManager::GetInstance().ReceiveData()) {
        sceneManager.ChangeScene(GAME, "");
    }
}

void WaitingScene::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText(waitingMessage.c_str(), 100, 300, 30, DARKGRAY);

    EndDrawing();
}

void WaitingScene::Unload() {}