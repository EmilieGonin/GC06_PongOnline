#pragma once

#include "Scene.h"

enum SceneType { MENU, GAME, END };

class SceneManager {
private:
    Scene* currentScene;

public:
    SceneManager();
    ~SceneManager();

    void ChangeScene(SceneType type);
    void Update();
    void Draw();
};

