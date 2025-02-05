#include "MenuScene.h"

#include "SceneManager.h"
#include "GameScene.h"
#include <cstring>

extern SceneManager* sceneManager;

MenuScene::MenuScene() {
    strcpy_s(username, sizeof(username), "Joueur");
    strcpy_s(serverIP, sizeof(serverIP), "127.0.0.1");
    editUser = false;
    editIP = false;
}

MenuScene::~MenuScene() {}

void MenuScene::Init() {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
}

void MenuScene::Update() {
    if (GuiButton(Rectangle{ 100, 350, 200, 60 }, "Creer une partie")) {
        sceneManager->ChangeScene(GAME);
    }

    if (GuiButton(Rectangle{ 350, 350, 200, 60 }, "Rejoindre une partie")) {
        sceneManager->ChangeScene(GAME);
    }
}

void MenuScene::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Menu Principal", 100, 50, 30, BLACK);
    DrawText("Pseudo:", 100, 120, 20, BLACK);
    GuiTextBox(Rectangle{ 200, 115, 200, 30 }, username, 20, editUser);
    DrawText("IP Serveur:", 100, 170, 20, BLACK);
    GuiTextBox(Rectangle{ 200, 165, 200, 30 }, serverIP, 20, editIP);
    EndDrawing();
}

void MenuScene::Unload() {}
