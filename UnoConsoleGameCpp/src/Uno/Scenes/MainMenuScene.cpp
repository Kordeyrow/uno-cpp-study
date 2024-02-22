#include <iostream>
#include <conio.h>
#include <vector>
#include <Uno/Scenes/MainMenuScene.h>
#include <ConsoleCore/UserInterface/UserInterface.h>
#include <ConsoleCore/UserInterface/UserOption.h>

auto MainMenuScene::Init() -> void
{

}

auto MainMenuScene::Run() -> std::shared_ptr<BaseScene>
{
    // TODO: MainMenu Logic

    // Placeholder UserInterface test
    //
    UserInterface ui;
    ui.AddUserOption(std::vector<std::shared_ptr<UserOptionData>> {
        std::make_shared<UserOptionData>(
            "hi", 
            [](){ return nullptr; }
        ) 
    });
    ui.ReadOptionAndExecute();

    return shared_from_this();
}