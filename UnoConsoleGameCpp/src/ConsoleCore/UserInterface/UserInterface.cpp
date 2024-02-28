#include <sstream>
#include <thread>
#include <conio.h>
#include <iostream>
#include <windows.h>
#include "ConsoleCore/UserInterface/UserInterface.h"

// Keys
//
#define ARROW_KEY (char)224
#define KEY_UP 72
#define KEY_DOWN 80
#define ENTER_KEY '\r'

// Colors
//
#define SELECTION_HIGHLIGHT "\x1b[1;30;107m"
#define CHOSEN_HIGHLIGHT "\x1b[1;30;106m"
#define RESET_HIGHLIGHT "\x1b[0m"


void UserInterface::AddUserOptions(const std::vector<std::shared_ptr<UserOptionData>> optionDataList) {

	// Init optionText and 
	//
	std::stringstream optionText;
	int optionInputNumber = optionFromKey.size() + 1;

	for (const auto& optionData : optionDataList)
	{
		// Construct optionText
		//
		optionText << std::to_string(optionInputNumber);
		optionText << ". ";
		optionText << optionData->GetDescription();

		// Add option entry (key[char], value[std::unique_ptr<UserOption>>])
		//
		auto key = *std::to_string(optionInputNumber).c_str();
		optionFromKey[key] = std::make_unique<UserOption>(optionInputNumber,
			optionText.str(),
			optionData);
		AddUserOption(optionText.str());

		// Setup next
		//
		optionText.str(std::string());
		optionInputNumber++;
	}

	Draw();
}

void UserInterface::ClearOptions() {
	optionFromKey.clear();
	currentSelectedIndex = 0;
}

void UserInterface::ShowOptions(int selectedIndex, bool chosen)
{
	if (selectedIndex == -1)
		selectedIndex = currentSelectedIndex;

	// Build options text
	//
	std::stringstream optionsText;

	int currentIndex = 0;
	for (const auto& pair : optionFromKey)
	{
		auto key = pair.first;
		const auto& option = pair.second;

		if (currentIndex == selectedIndex)
		{
			if (chosen)
				optionsText << CHOSEN_HIGHLIGHT;
			else
				optionsText << SELECTION_HIGHLIGHT;
		}

		optionsText << option->text;

		if (currentIndex == selectedIndex)
			optionsText << RESET_HIGHLIGHT;

		if (currentIndex < optionFromKey.size() - 1)
			optionsText << '\n' << " ";

		currentIndex++;
	}

	// Display options
	//
	SetUserOptions(optionsText.str());
}

auto UserInterface::ReadInputLine() -> std::string const {
	std::cout << " > ";
	return  std::string(std::istreambuf_iterator<char>(std::cin), {});
}

auto UserInterface::ReadInputKey() -> char const {
	return _getch();
}


// Function to get the console width
void getConsoleSize(int& width, int& height) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}


// Function to print the pattern adjusted to the console width
void PrintCleanScreen() {
	int consoleWidth, consoleHeight;
	getConsoleSize(consoleWidth, consoleHeight);
	char fill = ' ';

	// Build one line of the pattern
	std::string line(consoleWidth, fill);

	// Create the entire output as one string
	std::string output;
	for (int i = 0; i < consoleHeight; i++) {
		output += line + "\n";
	}

	// Print the entire output in one operation
	std::cout << output;
}


auto UserInterface::ClearConsole() -> void const {
	//system("CLS");





	COORD cursorPosition;	
	cursorPosition.X = 0;	
	cursorPosition.Y = 0;	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
	PrintCleanScreen();
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
	/*std::cout << 
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"

		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"

		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n";

	std::cout <<
		"-          |          |          |          |          |          |          |          |          |          |\n"
		"-          |          |          |          |          |          |          |          |          |          |\n"*/
	//std::cout << "                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n                                                          \n";





	//CONSOLE_SCREEN_BUFFER_INFO csbi;
	//COORD cursorPosition;
	//HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	//DWORD charsWritten;
	//DWORD consoleSize;

	//// Get the number of character cells in the current buffer
	//if (!GetConsoleScreenBufferInfo(stdHandle, &csbi)) {
	//	// Handle the error if necessary
	//	return;
	//}

	//consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

	//// Fill the entire screen with blanks
	//FillConsoleOutputCharacter(
	//	stdHandle,          // Handle to console screen buffer
	//	(TCHAR)' ',        // Character to write to the buffer
	//	consoleSize,        // Number of cells to write to
	//	{ 0, 0 },             // Coordinates of first cell
	//	&charsWritten       // Receive number of characters written
	//);

	//// Get the current text attribute
	//if (!GetConsoleScreenBufferInfo(stdHandle, &csbi)) {
	//	// Handle the error if necessary
	//	return;
	//}

	//// Set the buffer's attributes accordingly
	//FillConsoleOutputAttribute(
	//	stdHandle,          // Handle to console screen buffer
	//	csbi.wAttributes,   // Character attributes to use
	//	consoleSize,        // Number of cells to set attribute
	//	{ 0, 0 },             // Coordinates of first cell
	//	&charsWritten       // Receive number of characters written
	//);

	//// Put the cursor at its home coordinates
	//cursorPosition.X = 0;
	//cursorPosition.Y = 0;
	//SetConsoleCursorPosition(stdHandle, cursorPosition);
}

char UserInterface::GetChar() {
	return _getch();
}

bool UserInterface::ArrowMoveSelection(char input)
{
	if (input == ARROW_KEY) {
		// Move selected index
		//
		auto input = _getch();
		int moveSelectedIndex =
			(input == KEY_UP) ? -1
			: (input == KEY_DOWN) ? +1
			: 0;
		if (moveSelectedIndex != 0) {
			if (currentSelectedIndex + moveSelectedIndex == -1) {
				currentSelectedIndex = optionFromKey.size() - 1;
			}
			else {
				currentSelectedIndex = (currentSelectedIndex + moveSelectedIndex) % optionFromKey.size();
			}
			SetUserMessage("");
			return true;
		}
	}
	return false;
}

void UserInterface::ReadOptionAndExecute(char input) {

	ShowOptions(currentSelectedIndex);

	// Input
	//
	if (input == -1) {
		input = ReadInputKey();
	}
	int chosenOptionIndex = -1;

	// Change selection
	//
	if (ArrowMoveSelection(input))
		return;

	// Execute chosen
	//
	else if (input == ENTER_KEY) {
		chosenOptionIndex = currentSelectedIndex;
		for (const auto& pair : optionFromKey) {
			if (chosenOptionIndex == pair.second->inputNumberKey - 1) {
				input = pair.first;
				break;
			}
		}
	}

	if (chosenOptionIndex < 0) {
		// Check option chosen
		//
		if (optionFromKey.count(input)) {
			currentSelectedIndex = optionFromKey[input]->inputNumberKey - 1;
			chosenOptionIndex = currentSelectedIndex;
		}
	}

	// If option chosen
	//
	if (chosenOptionIndex >= 0) {

		// Hightlight option
		//
		ShowOptions(chosenOptionIndex, true);

		// Cleat user message
		//
		SetUserMessage("");

		// Wait delay
		std::this_thread::sleep_for(std::chrono::milliseconds(190));
		while (_kbhit())
			_getch();

		// Execute option
		//
		//std::cout << input << std::endl;
		optionFromKey[input]->userOptionData->Execute();
	}
	else {
		// Inform invalid option
		//
		SetUserMessage("Invalid selection. Please try again.");
	}
}

UserInterface UserInterface::GetState()
{
	return *this;
}

void UserInterface::SetState(UserInterface stateCopy)
{
	//return *this;
}

auto UserInterface::Draw() -> void
{

	// Build screenData
	// 
	std::stringstream screenData;
	screenData << " " << title << std::endl ;
	screenData << " " << scene << std::endl ;
	screenData << " " << userOptions << std::endl ;
	screenData << " " << userMessage;

	// Print screenData
	// 
	ClearConsole();
	std::cout << screenData.str();
}

auto UserInterface::SetTitle(const std::string& _title) -> void
{
	title = _title;
	Draw();
}
auto UserInterface::SetScene(const std::string& _scene) -> void
{
	scene = _scene;
	Draw();
}
auto UserInterface::SetUserOptions(const std::string& _userOptions) -> void
{
	userOptions = _userOptions;
	Draw();
}
auto UserInterface::SetUserMessage(const std::string& _userMessage) -> void
{
	userMessage = _userMessage;
	Draw();
}

auto UserInterface::AddTitle(const std::string& _title) -> void
{
	title = _title;
}
auto UserInterface::AddScene(const std::string& _scene) -> void
{
	scene = _scene;
}
auto UserInterface::AddUserOption(const std::string& _userOptions) -> void
{
	userOptions = _userOptions;
}
auto UserInterface::AddUserMessage(const std::string& _userMessage) -> void
{
	userMessage = _userMessage;
}
