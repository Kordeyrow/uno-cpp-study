#include "Uno/Match/Entities/Components/Hand.h"

int Hand::getHandSize() const {
    return deck.size();
}

std::string Hand::printHand() const {
    return std::string(getHandSize(), '*');
}
