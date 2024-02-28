#pragma once
#include <vector>
#include <string>
#include "Card.h"

class Hand {
public:
    std::vector<Card> deck = { 
        {"1", Card::GetRandomColorCode()},
        {"2", Card::GetRandomColorCode()},
        {"3", Card::GetRandomColorCode()},
        {"4", Card::GetRandomColorCode()},
    };
    int GetHandSize() const;
    std::string PrintHand() const;
};

