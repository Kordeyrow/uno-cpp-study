#include "Uno/Match/MatchSetupMenu.h"

MatchSetupMenu::MatchSetupMenu(
    std::shared_ptr<UserInterface> userInterface)
    : 
    userInterface{ userInterface }
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