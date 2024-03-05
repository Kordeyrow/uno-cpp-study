#include "Uno/Scenes/MainMenuScene.h"

MainMenuScene::MainMenuScene()
    : matchSetupMenu { userInterface, std::bind(&MainMenuScene::SetNextScene, this, std::placeholders::_1) }
{
}

auto MainMenuScene::Init() -> void
{
    userInterface->SetTitle("--[ MAIN MENU ]--");
    userInterface->AddUserOptions(matchSetupMenu.GetUserOptionDataList());
    nextScene = shared_from_this();
}

auto MainMenuScene::Run() -> std::shared_ptr<BaseScene>
{
    userInterface->ReadOptionAndExecute();

    return nextScene;
}

void MainMenuScene::SetNextScene(std::shared_ptr<BaseScene> _nextScene)
{
    nextScene = std::move(_nextScene);
}
