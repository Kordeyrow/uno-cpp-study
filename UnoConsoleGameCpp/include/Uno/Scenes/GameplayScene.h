#pragma once
#include <string>
#include <iostream>
#include "ConsoleCore/BaseScene/BaseScene.h"
#include <Uno/Match/Entities/Duelist.h>
#include <conio.h>
#include <algorithm> // For std::shuffle

struct ZeroWidthCharInfo {
    int rowIndex;
    int colIndex;
    int count;
};

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
    void PlayMatch();
    void DrawTable(UserInterface* ui, int duelist_index, bool winner);

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
        //for (size_t i = 0; i < 4; i++)
        //{
        //    // 2 cards of each
        //    //
        //    deck.emplace_back(
        //        Card::WILD_DRAW_4,
        //        Card::colors[i]
        //    );
        //    //std::cout << "draw+2/" << i << "  ";
        //}

        //_getch();

        return deck;
    }
    std::vector<Card> discardDeck;

private:

    int maxDuelists = 12;
    int minDuelists = 2;
    int startDuelists = 4;
    int duelistInitialHandSize = 1;
    int turnActionDelay = 1900;

    void DrawCard(std::vector<Card>& target);
    void PlayerOptionDrawCard();
    void DrawDuelist(
        const Duelist& duelist, 
        int x, 
        int y, 
        std::vector<std::string>& asciiTable, 
        bool highlight,
        bool winner,
        bool gameover);
    void PrintTopCard(int centerX, int centerY, std::vector<std::string>& asciiTable);
    //Card discardPileTopCard;
    bool endSetPlayers = false;
    int dir = 1;

    bool playerTurnActionDone = false;
    bool duelistTurnActionDone = false;

    // Exit
    bool exitGameplay;
    bool exitGameSetup;

    // Effects
    int cardsToBuy_2 = 0;
    int cardsToBuy_4 = 0;
    bool skip = false;

    std::shared_ptr<Duelist> winner;
    int winnerIndex;
    
    // Save how many zeroWidthCharacters exist in a line, marking each index
    // so that next draws knows how to move right correctly to draw in intended screen pos(pixel)
    std::vector<ZeroWidthCharInfo> zeroWidthCharactersByRow;
    
    //int moveRightCountDrawTable = 0;
    std::vector<std::shared_ptr<Duelist>> duelists;
    std::vector<Card> drawDeck = ShuffleDeck(CreateMatchDeck());

    std::vector<Card>& ShuffleDeck(std::vector<Card>& deck) {
        // Create a random number generator
        std::random_device rd; // Obtain a random number from hardware
        std::mt19937 g(rd()); // Seed the generator

        // Shuffle the cards
        std::shuffle(deck.begin(), deck.end(), g);
        return deck;
    }
};

