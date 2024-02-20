#include "MainMenuScene.h"
#include <iostream>

auto MainMenuScene::Init() -> void
{

}

auto MainMenuScene::Run() -> std::shared_ptr<BaseScene>
{
    // Step
    int n;
    std::cout << "Run" << std::endl;
    std::cin >> n;

    return shared_from_this();
}