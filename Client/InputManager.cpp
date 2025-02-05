#include "InputManager.h"

InputManager::InputManager() {
    user1Keys[0] = user1Keys[1] = false;
    user2Keys[0] = user2Keys[1] = false;
    mouseClicked = false;
    quitPressed = false;
}

void InputManager::Update() {
    // User 1 (Arrow Keys)
    user1Keys[0] = IsKeyDown(KEY_UP);    // Haut
    user1Keys[1] = IsKeyDown(KEY_DOWN);  // Bas

    // User 2 (Z, S)
    user2Keys[0] = IsKeyDown(KEY_Z);     // Haut
    user2Keys[1] = IsKeyDown(KEY_S);     // Bas

    // Mouse click detection
    mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    // Quit screen detection (Escape key)
    quitPressed = IsKeyPressed(KEY_ESCAPE);
}