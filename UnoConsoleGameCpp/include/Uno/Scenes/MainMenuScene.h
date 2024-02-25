#pragma once
#include "ConsoleCore/BaseScene/BaseScene.h"
#include "Uno/Match/MatchSetupMenu.h"
#include <string>
#include <functional>

class MainMenuScene : public BaseScene {
public:
    MainMenuScene();
    auto Run() -> std::shared_ptr<BaseScene> override;
    auto Init() -> void override;
    void SetNextScene(std::shared_ptr<BaseScene> nextScene);
private:
    MatchSetupMenu matchSetupMenu;
};

