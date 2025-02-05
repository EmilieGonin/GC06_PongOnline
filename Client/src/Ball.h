#pragma once

#include "Sprite.h"
#include <raylib.h>

class Ball {
public:
    Ball(float startX, float startY);
    void Update();
    void Draw() const;

    void InvertDirectionX();
    void InvertDirectionY();

    float GetX() const;
    float GetY() const;
    Vector2 GetSize() const;

private:
    Vector2 position;
    Vector2 velocity;
    float radius;

    Sprite* spriteBall;
};