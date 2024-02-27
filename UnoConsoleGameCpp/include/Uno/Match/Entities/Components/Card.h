#pragma once
#include <string>

class Card {

private:
	std::string description;

public:
	Card(const std::string& desc, int color) : description{ desc }, color{ color } {}

	std::string ColoredDescription() { return startColor + " " + description + " " + resetColor /*+ " " + colorBox*/; }

	int color = 31;

	std::string startColor = "\x1b[1;" + std::to_string(color + 10) + "m";
	//std::string startColorBackground = "\x1b[1;" + std::to_string(color) + ";""m";
	std::string resetColor = "\x1b[0m";

	//std::string startColor = "\x1b[1;" + std::to_string(color) + "m";
	std::string colorBox = "\x1b[1;" + std::to_string(color + 10) + "m  " + resetColor;
	//std::string resetColor = "\x1b[0m";
};