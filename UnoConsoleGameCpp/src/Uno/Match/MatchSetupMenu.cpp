#include "Uno/Match/MatchSetupMenu.h"
#include <Uno/Scenes/GameplayScene.h>

MatchSetupMenu::MatchSetupMenu(
    std::shared_ptr<UserInterface> userInterface,
    std::function<void(std::shared_ptr<BaseScene>)> _setNextScene)
    :
    userInterface{ userInterface },
    setNextScene{ _setNextScene }
{
    CreateUserActions();
}

void MatchSetupMenu::CreateUserActions()
{
    userOptionDataList.push_back(std::make_shared<UserOptionData>(
        "Play",
        std::bind(&MatchSetupMenu::StartGame, this)));

    userOptionDataList.push_back(std::make_shared<UserOptionData>(
        "View Rules",
        std::bind(&MatchSetupMenu::ViewRules, this)));

    userOptionDataList.push_back(std::make_shared<UserOptionData>(
        "Configure Match",
        std::bind(&MatchSetupMenu::ConfigureMatch, this)));

    userOptionDataList.push_back(std::make_shared<UserOptionData>(
        "Exit",
        std::bind(&MatchSetupMenu::Exit, this)));
}

void MatchSetupMenu::StartGame() {
    userInterface->SetScene("Starting game...");
    setNextScene(std::make_shared<GameplayScene>());
}

void MatchSetupMenu::ViewRules() {
    userInterface->SetScene("rules");
}

void MatchSetupMenu::ConfigureMatch() {
    userInterface->SetScene("Configuring up match...");
}

void MatchSetupMenu::Exit() {
    userInterface->SetScene("Exiting game...");
}