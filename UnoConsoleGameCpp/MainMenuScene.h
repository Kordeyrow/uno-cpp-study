#pragma once
#include "BaseScene.h"

class MainMenuScene : public BaseScene {
public:
    auto Run() -> std::shared_ptr<BaseScene> override;
    auto Init() -> void override;
};

