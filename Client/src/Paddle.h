#pragma once

#include "Sprite.h"
#include <raylib.h>

class Paddle {
public:
    Paddle(float startX, float startY);
    void Update(bool moveUp, bool moveDown);
    void Draw() const;

    float GetX() const;
    float GetY() const;
    Vector2 GetSize() const;

private:
    Vector2 position;
    float width;
    float height;
    float speed = 200;

    Sprite* spritePaddle;
};
