#include <iostream>
#include <raylib.h>

// This warning was triggered because Raylib is included statically
// Shouldn't cause any issues
#pragma warning(disable: 4098)

int main() {
    InitWindow(800, 600, "Raylib Test");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, Raylib!", 350, 280, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}