#include "Uno/Match/Entities/Components/Hand.h"

int Hand::GetHandSize() const {
    return deck.size();
}

std::string Hand::PrintHand() const {
    return std::string(GetHandSize(), '*');
}
