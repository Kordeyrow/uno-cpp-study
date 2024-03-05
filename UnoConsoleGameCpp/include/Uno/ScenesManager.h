#pragma once
#include "ConsoleCore/BaseScene/BaseScene.h"

class ScenesManager
{
public:
	ScenesManager();
	auto Run() -> void;
private:
	std::shared_ptr<BaseScene> currentScene;
};