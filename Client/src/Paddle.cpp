#include "Paddle.h"
#include <iostream>

Paddle::Paddle(float startX, float startY) {
    position.x = startX;
    position.y = startY;
    spritePaddle = new Sprite();
    if (!spritePaddle->LoadImage("paddle.png")) {  // Charge l'image de la raquette
        std::cerr << "Erreur de chargement de paddle.png" << std::endl;
    }
    spritePaddle->SetSize(20, 100);  // Taille de la raquette
    spritePaddle->SetPosition(startX, startY);
    height = spritePaddle->GetSize().second;
    width = spritePaddle->GetSize().first;
}

void Paddle::Update(bool moveUp, bool moveDown) {
    if (moveUp)
    {
        position.y -= speed * GetFrameTime();
    }
    if (moveDown) position.y += speed * GetFrameTime();

    // Limites de l'écran
    if (position.y < 0) position.y = 0;
    if (position.y + height > GetScreenHeight() - 28) position.y = GetScreenHeight() - 28 - height;

    spritePaddle->SetPosition(position.x, position.y);
}

void Paddle::Draw() const {
    spritePaddle->Draw();
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