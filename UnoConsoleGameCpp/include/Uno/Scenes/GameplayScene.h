#pragma once
#include <string>
#include <iostream>
#include "ConsoleCore/BaseScene/BaseScene.h"
#include <Uno/Match/Entities/Duelist.h>

class GameplayScene : public BaseScene {
public:
    GameplayScene();
    auto Run() -> std::shared_ptr<BaseScene> override;
    auto Init() -> void override;

    // ==========  Match Config  ========== //
    //
    void IncreaseTotalDuelists();
    void DecreaseTotalDuelists();
    void SetPlayersNames();
    void BackFromSetPlayersNames();

    // ==========  Play  ========== //
    //
    void Play();
    void DrawTable(UserInterface* ui);

    std::vector<Card>& CreateMatchDeck() {

        std::vector<Card>& deck = *new std::vector<Card>();

        for (size_t i = 0; i < 80; i++)
        {

        }

        return deck;
    }

private:
    int maxDuelists = 12;
    int minDuelists = 2;
    int duelistInitialHandSize = 7;
    bool endSetPlayers = false;
    std::vector<std::shared_ptr<Duelist>> duelists;
    std::vector<Card> matchDeck = CreateMatchDeck();
};

