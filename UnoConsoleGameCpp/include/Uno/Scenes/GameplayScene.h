#pragma once
#include <string>
#include "ConsoleCore/BaseScene/BaseScene.h"
#include <Uno/Match/Entities/Duelist.h>

class GameplayScene : public BaseScene {
public:
    GameplayScene();
    auto Run() -> std::shared_ptr<BaseScene> override;
    auto Init() -> void override;
    void BackFromSetPlayersNames();
    void SetPlayersNames();
    void IncreaseTotalDuelists();
    void DecreaseTotalDuelists();
    void Play();
private:
    int maxDuelists = 12;
    int minDuelists = 2;
    std::vector<std::shared_ptr<Duelist>> duelists;
    bool endSetPlayers = false;
};

