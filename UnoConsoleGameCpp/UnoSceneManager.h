#pragma once
#include "BaseScene.h"

class UnoSceneManager 
{
public:
	UnoSceneManager();
	auto Run() -> void;
private:
	std::shared_ptr<BaseScene> currentScene;
};