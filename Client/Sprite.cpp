#include "Sprite.h"
#include <iostream>

Sprite::Sprite() : positionX(0), positionY(0), width(0), height(0) {}

Sprite::~Sprite() {
    UnloadTexture(texture);
}

bool Sprite::LoadImage(const std::string& filepath) {
    texture = LoadTexture(filepath.c_str());
    if (texture.id == 0) {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        return false;
    }
    return true;
}

void Sprite::Draw() const {
    DrawTextureEx(texture, Vector2{ positionX, positionY }, 0.0f, 1.0f, WHITE);
}

void Sprite::SetPosition(float x, float y) {
    positionX = x;
    positionY = y;
}

std::pair<float, float> Sprite::GetPosition() const {
    return { positionX, positionY };
}

void Sprite::SetSize(float w, float h) {
    width = w;
    height = h;
}

std::pair<float, float> Sprite::GetSize() const {
    return { width, height };
}