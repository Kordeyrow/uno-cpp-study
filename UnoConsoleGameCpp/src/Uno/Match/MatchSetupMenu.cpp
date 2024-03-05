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
        "Match",
        std::bind(&MatchSetupMenu::ConfigureMatch, this)));

    userOptionDataList.push_back(std::make_shared<UserOptionData>(
        "View Rules",
        std::bind(&MatchSetupMenu::ViewRules, this)));

    /*userOptionDataList.push_back(std::make_shared<UserOptionData>(
        "Configure Match",
        std::bind(&MatchSetupMenu::ConfigureMatch, this)));*/

    userOptionDataList.push_back(std::make_shared<UserOptionData>(
        "Exit",
        std::bind(&MatchSetupMenu::Exit, this)));
}

void MatchSetupMenu::StartGame() {
    //userInterface->SetScene("Starting game...");
}

void MatchSetupMenu::ViewRules() {
    userInterface->SetScene("Goal: Be the first duelist to play all your cards. \n"
        " Playing: You can play the card if it matches the one in the table. \n"
        " Draw: If you don't have any cards to play, you are forced to draw a card \n"
        " Special Cards: 'Numbered', 'Skip', 'Reverse', 'Draw Two'.");
}

void MatchSetupMenu::ConfigureMatch() {
    //userInterface->SetScene("Configuring up match...");
    setNextScene(std::make_shared<GameplayScene>());
}

void MatchSetupMenu::Exit() {
    //userInterface->SetScene("Exiting game...");
    setNextScene(nullptr);
}