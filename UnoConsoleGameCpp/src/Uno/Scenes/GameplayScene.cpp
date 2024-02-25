#include "Uno/Scenes/GameplayScene.h"

GameplayScene::GameplayScene()
    //: matchSetupMenu{ userInterface }
{
}

auto GameplayScene::Init() -> void
{
    userInterface->SetTitle("--[ GAMEPLAY ]--");

    //userInterface->AddUserOptions(matchSetupMenu.GetUserActions());
}

auto GameplayScene::Run() -> std::shared_ptr<BaseScene>
{
    userInterface->ReadOptionAndExecute();
    return shared_from_this();
}