#pragma once

#include <string>
#include <functional>
#include <ConsoleCore/BaseScene/BaseScene.h>


class UserOptionData {
public:
    UserOptionData(
        const std::string& desc, 
        std::function<std::shared_ptr<BaseScene>()> actionFunc);
    std::string GetDescription() const;
    virtual std::shared_ptr<BaseScene> Execute() const;
private:
    std::string description;
    std::function<std::shared_ptr<BaseScene>()> action;
};
