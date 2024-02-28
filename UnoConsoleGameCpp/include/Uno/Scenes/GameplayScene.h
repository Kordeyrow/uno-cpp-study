#pragma once
#include <string>
#include <iostream>
#include "ConsoleCore/BaseScene/BaseScene.h"
#include <Uno/Match/Entities/Duelist.h>
#include <conio.h>

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

        // numbered cards (80 total), 20 for each color
        //
        for (size_t i = 0; i < 4; i++)
        {
            // 2 cards of each number 0-9
            //
            for (size_t j = 0; j < 20; j++)
            {
                deck.emplace_back(
                    Card::NUMBERED,
                    Card::colors[i],
                    j % 10
                );
                //std::cout << std::to_string(j % 10) + "/" << i << "  ";
            }
        }

        // "draw+2" cards (8 total), 2 for each color
        //
        for (size_t i = 0; i < 4; i++)
        {
            // 2 cards of each
            //
            for (size_t j = 0; j < 2; j++)
            {
                deck.emplace_back(
                    Card::DRAW_2,
                    Card::colors[i]
                );
                //std::cout << "draw+2/" << i << "  ";
            }
        }

        // "reverse" cards (8 total), 2 for each color
        //
        for (size_t i = 0; i < 4; i++)
        {
            // 2 cards of each
            //
            for (size_t j = 0; j < 2; j++)
            {
                deck.emplace_back(
                    Card::REVERSE,
                    Card::colors[i]
                );
                //std::cout << "draw+2/" << i << "  ";
            }
        }

        // "skip" cards (8 total), 2 for each color
        //
        for (size_t i = 0; i < 4; i++)
        {
            // 2 cards of each
            //
            for (size_t j = 0; j < 2; j++)
            {
                deck.emplace_back(
                    Card::SKIP,
                    Card::colors[i]
                );
                //std::cout << "draw+2/" << i << "  ";
            }
        }

        // "wild_draw+4" cards (4 total), 1 for each color
        //
        for (size_t i = 0; i < 4; i++)
        {
            // 2 cards of each
            //
            deck.emplace_back(
                Card::WILD_DRAW_4,
                Card::colors[i]
            );
            //std::cout << "draw+2/" << i << "  ";
        }

        //_getch();

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

