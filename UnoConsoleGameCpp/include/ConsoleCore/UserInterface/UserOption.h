#pragma once
#include <string>
#include <memory>
#include "UserOptionData.h"

class UserOption {
public:
	UserOption(
		int inputNumberKey,
		const std::string& text,
		std::shared_ptr<UserOptionData> userOptionData);
public:
	int inputNumberKey;
	std::string text;
	std::shared_ptr<UserOptionData> userOptionData;
};

