#pragma once

#include "Scene.h"

enum SceneType { MENU, GAME };

class SceneManager {
public:
    static SceneManager& GetInstance();

    void ChangeScene(SceneType type);
    void Update();
    void Draw();

private:
    SceneManager();                  
    ~SceneManager();                 
    SceneManager(const SceneManager&) = delete;         
    SceneManager& operator=(const SceneManager&) = delete;

    Scene* currentScene;
};