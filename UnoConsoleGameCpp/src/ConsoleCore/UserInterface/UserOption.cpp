#include "ConsoleCore/UserInterface/UserOption.h"

UserOption::UserOption(
	int inputNumberKey,
	const std::string& text,
	std::shared_ptr<UserOptionData> userOptionData)
	: 
	inputNumberKey{ inputNumberKey }, 
	text{ text }, 
	userOptionData{ userOptionData }
{
}
