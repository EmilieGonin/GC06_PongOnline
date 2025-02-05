#include "App.h"

void App::Run() {
    window.createWindow(800, 600, "Pong Online");

    while (window.isOpen()) {
        HandleEvents();
        Render();
    }

    window.close();
}

void App::HandleEvents() {
    // TODO: Handle player input events here
}

void App::Render() {
    window.clear();
    sceneManager.Update();
    sceneManager.Draw();
    window.display();
}

// main.cpp
#include "App.h"

int main() {
    App app;
    app.Run();
    return 0;
}
