#pragma once
#include <vector>
#include <string>
#include "Card.h"

class Hand {
public:
    std::vector<Card> deck = { {"7", 31}, {"2", 32} };
    int getHandSize() const;
    std::string printHand() const;
};

