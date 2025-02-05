#include "Paddle.h"

Paddle::Paddle(float startX, float startY) : position{ startX, startY }, width(20.0f), height(100.0f), speed(300.0f) {}

void Paddle::Update(bool moveUp, bool moveDown) {
    if (moveUp) position.y -= speed * GetFrameTime();
    if (moveDown) position.y += speed * GetFrameTime();

    // Limites de l'écran
    if (position.y < 0) position.y = 0;
    if (position.y + height > GetScreenHeight()) position.y = GetScreenHeight() - height;
}

void Paddle::Draw() const {
    DrawRectangle(position.x, position.y, width, height, WHITE);
}

float Paddle::GetX() const {
    return position.x;
}

float Paddle::GetY() const {
    return position.y;
}

Vector2 Paddle::GetSize() const {
    return { width, height };
}