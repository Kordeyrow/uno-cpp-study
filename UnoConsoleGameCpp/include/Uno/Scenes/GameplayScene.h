#pragma once
#include "ConsoleCore/BaseScene/BaseScene.h"

class GameplayScene : public BaseScene {
public:
    GameplayScene();
    auto Run() -> std::shared_ptr<BaseScene> override;
    auto Init() -> void override;
private:
    //MatchSetupMenu matchSetupMenu;
};

