#pragma once
#include <iostream>
#include <string>

#include "json.hpp"
#include "GameData.h"

using json = nlohmann::json;

class JsonParser
{
public:
    static std::unique_ptr<GameData> Parse(const json& j);

    static json Create(const GameState&);
    static json Create(const PlayerInput&);
    static json Create(const SimpleGameState&);

private:
    static PlayerInput ParsePlayerInput(const json& j);
    static GameState ParseGameState(const json& j);
    static SimpleGameState ParseSimpleGameState(const json& j);
};