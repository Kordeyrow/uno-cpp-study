#pragma once
#include "ConsoleCore/UserInterface/UserOptionHolder.h"
#include "ConsoleCore/UserInterface/UserInterface.h"

class MatchSetupMenu : public UserOptionHolder {
public:
    MatchSetupMenu(
        std::shared_ptr<UserInterface> userInterface);
private:
    void CreateUserActions();
    void StartGame();
    void ViewRules();
    void ConfigureMatch();
    void Exit();
    std::shared_ptr<UserInterface> userInterface;
};
