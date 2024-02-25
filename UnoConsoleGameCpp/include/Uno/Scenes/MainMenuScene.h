#pragma once
#include "ConsoleCore/BaseScene/BaseScene.h"
#include "Uno/Match/MatchSetupMenu.h"

class MainMenuScene : public BaseScene {
public:
    MainMenuScene();
    auto Run() -> std::shared_ptr<BaseScene> override;
    auto Init() -> void override;
private:
    MatchSetupMenu matchSetupMenu;
};

