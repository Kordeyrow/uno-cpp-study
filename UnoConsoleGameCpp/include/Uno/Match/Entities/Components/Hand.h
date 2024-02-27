#pragma once
#include <vector>
#include <string>
#include "Card.h"

constexpr auto RED = 31;
constexpr auto GREEN = 32;
constexpr auto YELLOW = 33;
constexpr auto BLUE = 34;

class Hand {
public:
    std::vector<Card> deck = { 
        {"1", RED},
        {"2", GREEN},
        {"3", YELLOW},
        {"4", BLUE},
    };
    int getHandSize() const;
    std::string printHand() const;
};

