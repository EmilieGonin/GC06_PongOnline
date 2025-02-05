#pragma once

#include "Scene.h"
#include "raylib.h"
#include "raygui.h"

class MenuScene : public Scene {
private:
    char username[20];
    char serverIP[20];
    bool editUser;
    bool editIP;

public:
    MenuScene();
    ~MenuScene();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Unload() override;
};

