#pragma once
#include "ConsoleCore/UserInterface/UserOptionHolder.h"
#include "ConsoleCore/UserInterface/UserInterface.h"
#include "ConsoleCore/BaseScene/BaseScene.h"
#include <functional>

class MatchSetupMenu : public UserOptionHolder {
public:
    MatchSetupMenu(
        std::shared_ptr<UserInterface> userInterface,
        std::function<void(std::shared_ptr<BaseScene>)> setNextScene);
private:
    void CreateUserActions();
    void StartGame();
    void ViewRules();
    void ConfigureMatch();
    void Exit();
    std::shared_ptr<UserInterface> userInterface;
    std::function<void(std::shared_ptr<BaseScene>)> setNextScene;
};
