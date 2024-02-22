#include <string>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <iostream>
#include <conio.h>
#include <iostream>
#include <ConsoleCore/UserInterface/UserInterface.h>

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


void UserInterface::AddUserOption(const std::vector<std::shared_ptr<UserOptionData>> optionDataList) {

	// Init optionText and 
	//
	std::stringstream optionText;
	int optionInputNumber = optionFromKey.size() + 1;

	for (const auto& optionData : optionDataList)
	{
		// Construct optionText
		//
		optionText << " ";
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
}

void UserInterface::ShowOptions(int selectedIndex, bool chosen = false)
{
	// Build options text
	//
	std::stringstream optionsText;
	optionsText << '\n';

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

		optionsText << option->text << '\n';

		if (currentIndex == selectedIndex)
			optionsText << RESET_HIGHLIGHT;

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

auto UserInterface::ClearConsole() -> void const {
	system("CLS");
}

std::shared_ptr<BaseScene> UserInterface::ReadOptionAndExecute() {

	ShowOptions(currentSelectedIndex);

	// Read choise or arrow
	//
	auto input = ReadInputKey();
	int chosenOptionIndex = -1;

	// If "Arrow" input
	//
	if (input == ARROW_KEY) {
		// Move selected index
		//
		auto input = _getch();
		int moveSelectedIndex =
			(input == KEY_UP) ? -1
			: (input == KEY_DOWN) ? +1
			: 0;
		if (moveSelectedIndex != 0) {
			currentSelectedIndex = (currentSelectedIndex + moveSelectedIndex) % optionFromKey.size();
			SetUserMessage("");
			return nullptr;
			//return nullptr;
		}
	}
	// If "Enter" input
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
		std::cout << input << std::endl;
		return optionFromKey[input]->userOptionData->Execute();
	}
	else {
		// Inform invalid option
		//
		SetUserMessage("Invalid selection. Please try again.");
	}

	return nullptr;
}

auto UserInterface::Draw() -> void
{
	ClearConsole();

	// Build screenData
	// 
	std::stringstream screenData;
	screenData << title << std::endl << std::endl;
	screenData << scene << std::endl << std::endl;
	screenData << userOptions << std::endl << std::endl;
	screenData << userMessage << std::endl << std::endl;
	screenData << std::endl;

	// Print screenData
	// 
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
	title = '\n' + _title;
}
auto UserInterface::AddScene(const std::string& _scene) -> void
{
	scene = '\n' + _scene;
}
auto UserInterface::AddUserOption(const std::string& _userOptions) -> void
{
	userOptions = '\n' + _userOptions;
}
auto UserInterface::AddUserMessage(const std::string& _userMessage) -> void
{
	userMessage = '\n' + _userMessage;
}
