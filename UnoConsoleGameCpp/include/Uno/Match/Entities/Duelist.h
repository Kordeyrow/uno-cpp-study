#pragma once
#include <string>
#include <memory>
#include "Components/Hand.h"

class Duelist {
public:
	Duelist(const std::string& name, int handSize) : name{ name }, handSize { handSize } {}
	int handSize = 0;
	std::string name;
	std::shared_ptr<Hand> hand = std::make_shared<Hand>();
};

