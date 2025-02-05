#pragma once

#include "Sprite.h"

class Paddle {
public:
    Paddle(float startX, float startY);
    void Update(bool moveUp, bool moveDown);
    void Draw() const;

    float GetX() const;
    float GetY() const;
    float GetWidth() const;
    float GetHeight() const;

private:
    Sprite sprite;
    float speed = 300.0f;
};