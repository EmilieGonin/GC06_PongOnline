#pragma once

class InputManager {
public:
    InputManager();
    ~InputManager() = default;

    void Update();

    // User 1
    bool IsUser1Up() const { return user1Keys[0]; }
    bool IsUser1Down() const { return user1Keys[1]; }

    // User 2
    bool IsUser2Up() const { return user2Keys[0]; }
    bool IsUser2Down() const { return user2Keys[1]; }

    // Mouse and Exit
    bool IsMouseClicked() const { return mouseClicked; }
    bool ShouldQuit() const { return quitPressed; }

private:
    bool user1Keys[2]; // Up, Down for User 1 (KEY_UP, KEY_DOWN)
    bool user2Keys[2]; // Up, Down for User 2 (KEY_Z, KEY_S)
    bool mouseClicked; // Detects mouse click
    bool quitPressed;  // Detects quit key (Escape)
};