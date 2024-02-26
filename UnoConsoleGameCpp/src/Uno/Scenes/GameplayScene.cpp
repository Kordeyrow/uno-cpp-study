#include "Uno/Scenes/GameplayScene.h"

GameplayScene::GameplayScene()
    //: matchSetupMenu{ userInterface }
{
}

auto GameplayScene::Init() -> void
{
    userInterface->SetTitle("--[ GAMEPLAY ]--");

    userInterface->AddUserOptions({

        std::make_shared<UserOptionData>(
            "Increase total duelists",
            std::bind(&GameplayScene::IncreaseTotalDuelists, this)),

        std::make_shared<UserOptionData>(
            "Decrease total duelists",
            std::bind(&GameplayScene::DecreaseTotalDuelists, this))
    });
}

auto GameplayScene::Run() -> std::shared_ptr<BaseScene>
{
    userInterface->SetScene("totalDuelists = " + std::to_string(totalDuelists));
    userInterface->ReadOptionAndExecute();
    return shared_from_this();
}

void GameplayScene::IncreaseTotalDuelists() {
    totalDuelists += 1;
    if (totalDuelists > 12)
        totalDuelists = 12;
}

void GameplayScene::DecreaseTotalDuelists() {
    totalDuelists -= 1;
    if (totalDuelists < 2)
        totalDuelists = 2;
}