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
	virtual ~UserOption() = default;
	std::unique_ptr<UserOption> Clone();
public:
	int inputNumberKey;
	std::string text;
	std::shared_ptr<UserOptionData> userOptionData;
};

