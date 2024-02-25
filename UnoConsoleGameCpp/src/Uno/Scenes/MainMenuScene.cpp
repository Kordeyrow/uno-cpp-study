#include "Uno/Scenes/MainMenuScene.h"

MainMenuScene::MainMenuScene() 
    : matchSetupMenu { userInterface } 
{
}

auto MainMenuScene::Init() -> void
{
    userInterface->SetTitle("--[ MAIN MENU ]--");
    userInterface->AddUserOptions(matchSetupMenu.GetUserActions());
}

auto MainMenuScene::Run() -> std::shared_ptr<BaseScene>
{
    userInterface->ReadOptionAndExecute();
    return shared_from_this();
}