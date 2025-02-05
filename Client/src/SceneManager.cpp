#include "SceneManager.h"

#include "MenuScene.h"
#include "GameScene.h"

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

void SceneManager::ChangeScene(SceneType type) {
    if (currentScene) {
        currentScene->Unload();
        delete currentScene;
    }

    if (type == MENU) {
        currentScene = new MenuScene();
    }
    else if (type == GAME) {
        currentScene = new GameScene();
    }
    currentScene->Init();
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