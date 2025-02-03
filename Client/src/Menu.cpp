#include "Menu.h"
#include "raygui.h"
#include <cstring>

Menu::Menu() {
    strcpy_s(username, sizeof(username), "Joueur");
    strcpy_s(serverIP, sizeof(serverIP), "127.0.0.1");
    editUser = false;
    editIP = false;
}

Menu::~Menu() {
    CloseWindow();
}

void Menu::Run() {
    InitWindow(900, 600, "Pong Net - Menu");  
    SetTargetFPS(60);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    while (!WindowShouldClose()) {
        HandleInput();
        Draw();
    }
}

void Menu::HandleInput() {
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
            if ((key >= '0' && key <= '9') || key == '.' && len < 19) {
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

void Menu::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Pseudo :", 100, 80, 30, BLACK);  
    if (GuiTextBox(Rectangle{ 100, 120, 300, 50 }, username, 20, editUser)) {  
        editUser = !editUser;
        editIP = false;
    }

   
    DrawText("Adresse IP :", 100, 200, 30, BLACK);
    if (GuiTextBox(Rectangle{ 100, 240, 300, 50 }, serverIP, 20, editIP)) {
        editIP = !editIP;
        editUser = false;
    }

   
    if (GuiButton(Rectangle{ 100, 350, 200, 60 }, "Creer une partie")) {
      
    }

    if (GuiButton(Rectangle{ 350, 350, 200, 60 }, "Rejoindre une partie")) {
       
    }

    EndDrawing();
}
