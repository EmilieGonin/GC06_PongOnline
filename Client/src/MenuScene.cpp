#include "MenuScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include <cstring>
#include <string>


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
    HandleInput();  // Gérer les entrées utilisateur

    // Gestion des boutons
    if (GuiButton(Rectangle{ 100, 450, 200, 60 }, "Creer une partie")) {
        SceneManager::GetInstance().ChangeScene(GAME);
    }

    if (GuiButton(Rectangle{ 100, 520, 200, 60 }, "Rejoindre une partie")) {
        SceneManager::GetInstance().ChangeScene(GAME);
    }

    // Gestion des champs de texte
    if (GuiTextBox(Rectangle{ 300, 115, 200, 30 }, username, 20, editUser)) {
        editUser = !editUser;
        editIP = false; // Désactive l'édition de l'IP
    }

    if (GuiTextBox(Rectangle{ 300, 165, 200, 30 }, serverIP, 20, editIP)) {
        editIP = !editIP;
        editUser = false; // Désactive l'édition du pseudo
    }
}


void MenuScene::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Titre du menu
    DrawText("Menu Principal", 100, 50, 30, BLACK);

    // Affichage des labels
    DrawText("Pseudo:", 100, 120, 20, BLACK);
    //GuiTextBox(Rectangle{ 300, 115, 200, 30 }, username, 20, editUser);

    DrawText("IP Serveur:", 100, 170, 20, BLACK);
    //GuiTextBox(Rectangle{ 300, 165, 200, 30 }, serverIP, 20, editIP);

    // Affichage des boutons
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
            if ((key >= 32) && (key <= 125) && (len < 19)) { // Autorise toutes les lettres et symboles visibles
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
            if (((key >= '0' && key <= '9') || key == '.') && (len < 19)) { // Autorise uniquement chiffres et points
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