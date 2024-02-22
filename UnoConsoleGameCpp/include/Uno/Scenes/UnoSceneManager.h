#pragma once
#include <ConsoleCore/BaseScene/BaseScene.h>

class UnoSceneManager 
{
public:
	UnoSceneManager();
	auto Run() -> void;
private:
	std::shared_ptr<BaseScene> currentScene;
};