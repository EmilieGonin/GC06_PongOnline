#include "InputManager.h"
//#include <raylib.h>
#include <winsock2.h>


InputManager::InputManager() {
    user1Keys[0] = user1Keys[1] = false;
    user2Keys[0] = user2Keys[1] = false;
    mouseClicked = false;
    quitPressed = false;
}

void InputManager::Update() {
    // User 1 (Arrow Keys)
    user1Keys[0] = GetAsyncKeyState('Z') & 0x8000;    // Haut
    user1Keys[1] = GetAsyncKeyState('S') & 0x8000;  // Bas

    // User 2 (Z, S)
    user2Keys[0] = GetAsyncKeyState('E') & 0x8000;    // Haut     // Haut
    user2Keys[1] = GetAsyncKeyState('D') & 0x8000;    // Haut     // Bas

    // Mouse click detection
    //mouseClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    // Quit screen detection (Escape key)
    //quitPressed = IsKeyPressed(KEY_ESCAPE);
}