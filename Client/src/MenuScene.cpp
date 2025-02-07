#include "MenuScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include <cstring>
#include <string>
#include <iostream>

MenuScene::MenuScene()
{
    strcpy_s(username, sizeof(username), "Joueur");
    strcpy_s(serverIP, sizeof(serverIP), "127.0.0.1");
    editUser = false;
    editIP = false;

    NetworkManager::GetInstance().Initialize();
}

MenuScene::~MenuScene() {
    NetworkManager::GetInstance().Shutdown(); 
}

void MenuScene::Init() {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
}

void MenuScene::Update() {
    HandleInput();
    SceneManager& sceneManager = SceneManager::GetInstance();

    if (GuiButton(Rectangle{ 100, 450, 200, 60 }, "Creer une partie")) {
        sceneManager.SetUsername(username);
        sceneManager.SetServerIP(serverIP);
        sceneManager.SetPlayerID(1);

        if (NetworkManager::GetInstance().SendData(1, 1))
        {
            sceneManager.ChangeScene(WAITING, "En attente d'un autre joueur...");
        }
    }

    if (GuiButton(Rectangle{ 100, 520, 200, 60 }, "Rejoindre une partie")) {
        sceneManager.SetUsername(username);
        sceneManager.SetServerIP(serverIP);
        sceneManager.SetPlayerID(2);

        if (NetworkManager::GetInstance().SendData(1, 2))
        {
            sceneManager.ChangeScene(WAITING, "Connexion en cours...");
        }
    }

    if (GuiTextBox(Rectangle{ 300, 115, 200, 30 }, username, 20, editUser)) {
        editUser = !editUser;
        editIP = false;
    }

    if (GuiTextBox(Rectangle{ 300, 165, 200, 30 }, serverIP, 20, editIP)) {
        editIP = !editIP;
        editUser = false;
    }
}

void MenuScene::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Menu Principal", 100, 50, 30, BLACK);
    DrawText("Pseudo:", 100, 120, 20, BLACK);
    DrawText("IP Serveur:", 100, 170, 20, BLACK);

    GuiButton(Rectangle{ 100, 450, 200, 60 }, "Creer une partie");
    GuiButton(Rectangle{ 100, 520, 200, 60 }, "Rejoindre une partie");

    EndDrawing();
}

void MenuScene::Unload() {}

void MenuScene::HandleInput() {
    if (editUser) {
        int key = GetCharPressed();
        int len = strlen(username);
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (len < 19)) {
                username[len] = (char)key;
                username[len + 1] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && len > 0) {
            username[len - 1] = '\0';
        }
    }

    if (editIP) {
        int key = GetCharPressed();
        int len = strlen(serverIP);
        while (key > 0) {
            if (((key >= '0' && key <= '9') || key == '.') && (len < 19)) {
                serverIP[len] = (char)key;
                serverIP[len + 1] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && len > 0) {
            serverIP[len - 1] = '\0';
        }
    }
}
