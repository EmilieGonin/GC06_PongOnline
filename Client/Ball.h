#pragma once

#include "Sprite.h"

class Ball {
public:
    Ball(float startX, float startY);
    void Update();
    void Draw() const;

    void InvertDirectionX();
    void InvertDirectionY();

private:
    Sprite sprite;
    float dirX = 200.0f;
    float dirY = 200.0f;
};