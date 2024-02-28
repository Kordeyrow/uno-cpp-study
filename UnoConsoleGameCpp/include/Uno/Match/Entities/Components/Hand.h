#pragma once
#include <vector>
#include <string>
#include "Card.h"

class Hand {
public:
    std::vector<Card> deck = {
        Card {
            Card::NUMBERED,
            Card::GetRandomColorCode(),
            3
        },
        Card {
            Card::DRAW_2,
            Card::GetRandomColorCode()
        },
        Card {
            Card::REVERSE,
            Card::GetRandomColorCode()
        },
        Card {
            Card::SKIP,
            Card::GetRandomColorCode()
        },
        Card {
            Card::WILD_DRAW_4,
            Card::GetRandomColorCode()
        },
    };
    int GetHandSize() const;
    std::string PrintHand() const;
};

