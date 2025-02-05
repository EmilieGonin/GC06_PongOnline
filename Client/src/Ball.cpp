#include "Ball.h"

Ball::Ball(float startX, float startY) : position{ startX, startY }, velocity{ 200.0f, 200.0f }, radius(10.0f) {}

void Ball::Update() {
    position.x += velocity.x * GetFrameTime();
    position.y += velocity.y * GetFrameTime();

    // Collision avec le haut et le bas de l'écran
    if (position.y <= 0 || position.y + radius * 2 >= GetScreenHeight()) {
        InvertDirectionY();
    }
}

void Ball::Draw() const {
    DrawCircle(position.x, position.y, radius, WHITE);
}

void Ball::InvertDirectionX() {
    velocity.x = -velocity.x;
}

void Ball::InvertDirectionY() {
    velocity.y = -velocity.y;
}

float Ball::GetX() const {
    return position.x;
}

float Ball::GetY() const {
    return position.y;
}

Vector2 Ball::GetSize() const {
    return { radius * 2, radius * 2 };
}