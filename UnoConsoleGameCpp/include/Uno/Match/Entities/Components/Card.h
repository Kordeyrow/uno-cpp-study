﻿#pragma once
#include <string>
#include <random>



class Card {

private:
	std::string description;

public:

	// Color IDs
	//
	constexpr static int RED = 31;
	constexpr static int GREEN = 32;
	constexpr static int YELLOW = 33;
	constexpr static int BLUE = 34;
	constexpr static int NONE = 0;
	constexpr static int ALL = -1;

	// Type IDs (description)
	//
	constexpr static int NUMBERED = 1;
	constexpr static int DRAW_2 = 2;
	constexpr static int REVERSE = 3;
	constexpr static int SKIP = 4;
	constexpr static int WILD_DRAW_4 = 5;

	int typeID;
	int colorID;
	int number = -1;

	Card(int typeID, int color, int number = -1)
		: typeID{ typeID }, colorID{ color }, number{ number }
	{
		if (typeID == NUMBERED) {
			description = std::to_string(number);
		}
		else if (typeID == DRAW_2) {
			description = "+2";
		}
		else if (typeID == REVERSE) {
			description = "Reverse";
		}
		else if (typeID == SKIP) {
			description = "Skip";
		}
		else if (typeID == WILD_DRAW_4) {
			description = "Wild +4";
		}
	}

	constexpr static int colors[4] = { RED, GREEN, YELLOW, BLUE };


	static std::string DoColorBGText(std::string s, int colorID) {
		colorID += 10; // +10 because background ranges grom 40 to 49 / foreground 30 to 39
		std::string colorCSI = "\x1b[1;" + std::to_string(colorID) + "m"; // +10 because background ranges grom 40 to 49 / foreground 30 to 39
		std::string resetColorCSI = "\x1b[0m";
		return colorCSI + s + resetColorCSI;
	}
	
	std::string RawDescription() { return " " + description + " "; }
	std::string ColoredDescription() { return ColorCSI() + RawDescription() + ResetColorCSI(); }
	//std::string ColoredDescriptionFixed() { return startColor + " " + description + " " + resetColor; }
	std::string ColorCSI() { return "\x1b[1;" + std::to_string(colorID + 10) + "m"; } // +10 because background ranges grom 40 to 49 / foreground 30 to 39
	std::string ResetColorCSI() { return "\x1b[0m"; }

	static int GetRandomColorCode() {
        static std::random_device rd; // Obtain a random number from hardware
        static std::mt19937 gen(rd()); // Seed the generator
        std::uniform_int_distribution<> distr(0, 3); // Define the range
        int colors[4] = {RED, GREEN, YELLOW, BLUE};
        int index = distr(gen); // Generate a random index
        return colors[index];
    }
};