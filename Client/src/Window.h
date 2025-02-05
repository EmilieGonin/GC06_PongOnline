#pragma once

#include "raylib.h"

class Window {
private:
    bool open;

public:
    Window();
    ~Window();

    bool initialize();
    bool createWindow(int width, int height, const char* title);
    bool isOpen();
    void clear();
    void display();
    void close();
};

