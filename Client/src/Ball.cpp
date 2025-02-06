#include "Ball.h"
#include <iostream>

Ball::Ball(float startX, float startY) {
    position.x = startX;
    position.y = startY;

    spriteBall = new Sprite();
    if (!spriteBall->LoadImage("ball.png")) {  // Charge l'image de la balle
        std::cerr << "Erreur de chargement de ball.png" << std::endl;
    }
    spriteBall->SetSize(20, 20);  // Taille de la balle
    spriteBall->SetPosition(startX, startY);
}

void Ball::Update() {
    position.x += velocity.x * GetFrameTime();
    position.y += velocity.y * GetFrameTime();

    // Collision avec le haut et le bas de l'écran
    if (position.y <= 0 || position.y + radius * 2 >= GetScreenHeight()) {
        InvertDirectionY();
    }

    spriteBall->SetPosition(position.x, position.y);
}

void Ball::Draw() const {
    spriteBall->Draw();
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