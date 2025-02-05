#pragma once
  
#include <vector>
#include "../SceneManager.h"
#include "../Window.h"

class App {
private:
    Window window;
    SceneManager sceneManager;

public:
    void Run();
    void HandleEvents();
    void Render();
};

