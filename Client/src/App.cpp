#define _CRT_SECURE_NO_WARNINGS
#define RAYGUI_IMPLEMENTATION

#include "App.h"
#include "SceneManager.h"
#include <raylib.h>
#include <raygui.h>

// This warning was triggered because Raylib is included statically
// Shouldn't cause any issues
#pragma warning(disable: 4098)

void App::Run() {
    InitWindow(900, 600, "Pong Online");
    SetTargetFPS(60);

    // Initialisation du gestionnaire de sc�nes
    SceneManager& sceneManager = SceneManager::GetInstance();

    while (!WindowShouldClose()) {
        // Mise � jour de la sc�ne actuelle
        sceneManager.Update();

        // Rendu de la sc�ne actuelle
        sceneManager.Draw();
    }

    CloseWindow();
}

void App::HandleEvents()
{
	// TODO: Handle player input events here
}

void App::Update(float deltaTime)
{
}

void App::Render()
{
	BeginDrawing();
	ClearBackground(WHITE);

	DrawText("Hello, Raylib!", 350, 280, 20, DARKGRAY); //temp

	// TODO: Draw game objects here
	// Draw associated shape/sprite to each entity

	EndDrawing();
}