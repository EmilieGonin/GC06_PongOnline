#include "App.h"

#include <raylib.h>

// This warning was triggered because Raylib is included statically
// Shouldn't cause any issues
#pragma warning(disable: 4098)

void App::Run()
{
	InitWindow(800, 600, "Pong Online");

	while (!WindowShouldClose())
	{
		Render();
	}

	CloseWindow();
}

void App::HandleEvents()
{
	// TODO: Handle player input events here
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