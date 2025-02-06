#include "SceneManager.h"

#include "MenuScene.h"
#include "GameScene.h"
#include "WaitingScene.h"

SceneManager::SceneManager() {
    currentScene = new MenuScene();
    currentScene->Init();
}

SceneManager::~SceneManager() {
    delete currentScene;
}

SceneManager& SceneManager::GetInstance() {
    static SceneManager instance;  // Instance unique
    return instance;
}

void SceneManager::ChangeScene(SceneType type, const std::string& message = "") {
    if (currentScene) {
        currentScene->Unload();
        delete currentScene;
        currentScene = nullptr;
    }

    switch (type) {
    case MENU:
        currentScene = new MenuScene();
        break;
    case WAITING:
        currentScene = new WaitingScene(message);
        break;
    case GAME:
        currentScene = new GameScene();
        break;
    }

    if (currentScene) {
        currentScene->Init();
    }
}


void SceneManager::Update() {
    if (currentScene) {
        currentScene->Update();
    }
}

void SceneManager::Draw() {
    if (currentScene) {
        currentScene->Draw();
    }
}