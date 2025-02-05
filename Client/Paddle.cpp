#include "Paddle.h"

Paddle::Paddle(float startX, float startY) {
    sprite.LoadImage("paddle.png");
    sprite.SetSize(20, 100);
    sprite.SetPosition(startX, startY);
}

void Paddle::Update(bool moveUp, bool moveDown) {
    auto [x, y] = sprite.GetPosition();
    if (moveUp) y -= speed * GetFrameTime();
    if (moveDown) y += speed * GetFrameTime();

    // Limiter les mouvements à l'écran
    if (y < 0) y = 0;
    if (y + sprite.GetSize().second > GetScreenHeight()) y = GetScreenHeight() - sprite.GetSize().second;

    sprite.SetPosition(x, y);
}

void Paddle::Draw() const {
    sprite.Draw();
}

float Paddle::GetX() const { return sprite.GetPosition().first; }
float Paddle::GetY() const { return sprite.GetPosition().second; }
float Paddle::GetWidth() const { return sprite.GetSize().first; }
float Paddle::GetHeight() const { return sprite.GetSize().second; }
