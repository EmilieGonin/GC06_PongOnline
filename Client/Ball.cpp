#include "Ball.h"

Ball::Ball(float startX, float startY) {
    sprite.LoadImage("ball.png");
    sprite.SetSize(20, 20);
    sprite.SetPosition(startX, startY);
}

void Ball::Update() {
    auto [x, y] = sprite.GetPosition();
    x += dirX * GetFrameTime();
    y += dirY * GetFrameTime();

    // Collision avec les murs
    if (y <= 0 || y + sprite.GetSize().second >= GetScreenHeight()) {
        InvertDirectionY();
    }

    sprite.SetPosition(x, y);
}

void Ball::Draw() const {
    sprite.Draw();
}

void Ball::InvertDirectionX() {
    dirX = -dirX;
}

void Ball::InvertDirectionY() {
    dirY = -dirY;
}