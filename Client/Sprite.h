#pragma once

#include <raylib.h>
#include <string>
#include <utility>

class Sprite {
public:
    Sprite();
    ~Sprite();

    bool LoadImage(const std::string& filepath);
    void Draw() const;

    void SetPosition(float x, float y);
    std::pair<float, float> GetPosition() const;

    void SetSize(float width, float height);
    std::pair<float, float> GetSize() const;

private:
    Texture2D texture;
    float positionX, positionY;
    float width, height;
};
