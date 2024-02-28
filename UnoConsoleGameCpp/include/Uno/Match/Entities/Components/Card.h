#pragma once
#include <string>
#include <random>

constexpr auto RED = 31;
constexpr auto GREEN = 32;
constexpr auto YELLOW = 33;
constexpr auto BLUE = 34;


class Card {

private:
	std::string description;

public:
	Card(const std::string& desc, int color) : description{ desc }, color{ color } {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
	}

	std::string ColoredDescription() { return startColor + " " + description + " " + resetColor /*+ " " + colorBox*/; }

	int color = 31;

	std::string startColor = "\x1b[1;" + std::to_string(color + 10) + "m";
	//std::string startColorBackground = "\x1b[1;" + std::to_string(color) + ";""m";
	std::string resetColor = "\x1b[0m";

	//std::string startColor = "\x1b[1;" + std::to_string(color) + "m";
	std::string colorBox = "\x1b[1;" + std::to_string(color + 10) + "m  " + resetColor;
	//std::string resetColor = "\x1b[0m";

	static int GetRandomColorCode() {
        static std::random_device rd; // Obtain a random number from hardware
        static std::mt19937 gen(rd()); // Seed the generator
        std::uniform_int_distribution<> distr(0, 3); // Define the range
        int colors[4] = {RED, GREEN, YELLOW, BLUE};
        int index = distr(gen); // Generate a random index
        return colors[index];
    }
};