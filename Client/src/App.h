#pragma once

#include <vector>

class Window;
class SceneManager;

class App {
private:
    Window window;
    SceneManager sceneManager;

public:
    void Run();
    void HandleEvents();
    void Render();
};

