#include "Window.h"

Window::Window() : open(false) {}

Window::~Window() {
    close();
}

bool Window::initialize() {
    return true;
}

bool Window::createWindow(int width, int height, const char* title) {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(width, height, title);
    open = !WindowShouldClose();
    return open;
}

bool Window::isOpen() {
    return !WindowShouldClose();
}

void Window::clear() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
}

void Window::display() {
    EndDrawing();
}

void Window::close() {
    if (open) {
        CloseWindow();
        open = false;
    }
}