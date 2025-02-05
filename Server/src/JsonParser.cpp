#include "JsonParser.h"

std::unique_ptr<GameData> JsonParser::Parse(const json& j)
{
    int requestId = j.at("RequestId").get<int>();

    switch (requestId)
    {
    case 1:
        return std::make_unique<PlayerInput>(ParsePlayerInput(j));
    case 2:
        return std::make_unique<GameState>(ParseGameState(j));
    case 3:
        return std::make_unique<SimpleGameState>(ParseSimpleGameState(j));
    default:
        std::cout << "Unknown RequestId: " << requestId << std::endl;
        return nullptr;
    }
}

json JsonParser::Create(const GameState& gs)
{
    return json{
        {"BallX", gs.ballX},
        {"BallY", gs.ballY},
        {"VelocityX", gs.ballVelX},
        {"VelocityY", gs.ballVelY},
        {"Player1Y", gs.player1Y},
        {"Player2Y", gs.player2Y},
        {"Score1", gs.score1},
        {"Score2", gs.score2}
    };
}

json JsonParser::Create(const PlayerInput& pi)
{
    return json{
        {"MatchID", pi.matchID},
        {"PlayerID", pi.playerID},
        {"Up", pi.moveUp},
        {"Down", pi.moveDown}
    };
}

json JsonParser::Create(const SimpleGameState& sgs)
{
    return json{
        {"FrameID", sgs.frameID},
        {"Player1Y", sgs.player1Y},
        {"Player2Y", sgs.player2Y},
        {"Score1", sgs.score1},
        {"Score2", sgs.score2},
        {"BallX", sgs.ballx},
        {"BallY", sgs.bally}
    };
}

PlayerInput JsonParser::ParsePlayerInput(const json& j)
{
    return PlayerInput{
        j.at("MatchID").get<int>(),
        j.at("PlayerID").get<int>(),
        j.at("Up").get<bool>(),
        j.at("Down").get<bool>()
    };
}

GameState JsonParser::ParseGameState(const json& j)
{
    return GameState{
        j.at("BallX").get<float>(),
        j.at("BallY").get<float>(),
        j.at("VelocityX").get<float>(),
        j.at("VelocityY").get<float>(),
        j.at("Player1Y").get<float>(),
        j.at("Player2Y").get<float>(),
        j.at("Score1").get<int>(),
        j.at("Score2").get<int>()
    };
}

SimpleGameState JsonParser::ParseSimpleGameState(const json& j)
{
    return SimpleGameState{
        j.at("FrameID").get<int>(),
        j.at("Player1Y").get<float>(),
        j.at("Player2Y").get<float>(),
        j.at("Score1").get<int>(),
        j.at("Score2").get<int>(),
        j.at("BallX").get<float>(),
        j.at("BallY").get<float>()
    };
}