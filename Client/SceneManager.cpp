
#include "SceneManager.h"
#include "MenuScene.h"
#include "GameScene.h"

SceneManager::SceneManager() {
    currentScene = new MenuScene(); 
    currentScene->Init();
}

SceneManager::~SceneManager() {
    currentScene->Unload();
    delete currentScene;
}

void SceneManager::ChangeScene(SceneType type) {
    currentScene->Unload();
    delete currentScene;

    switch (type) {
    case MENU: currentScene = new MenuScene(); break;
    case GAME: currentScene = new GameScene(); break;
        // Ajoute END plus tard
    }

    currentScene->Init();
}

void SceneManager::Update() {
    currentScene->Update();
}

void SceneManager::Draw() {
    currentScene->Draw();
}
