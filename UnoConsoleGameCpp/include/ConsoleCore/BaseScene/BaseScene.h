#pragma once
#include <string>
#include "ConsoleCore/UserInterface/UserInterface.h"

class BaseScene : public std::enable_shared_from_this<BaseScene> {
public:
    virtual auto Init() -> void = 0;
    virtual auto Run() -> std::shared_ptr<BaseScene> = 0;
protected:
    auto SetNextScene(std::shared_ptr<BaseScene> _nextScene) -> void {
        nextScene = _nextScene;
    };
    std::string title;
    std::shared_ptr<UserInterface> userInterface = std::make_shared<UserInterface>();
    std::shared_ptr<BaseScene> nextScene;
};