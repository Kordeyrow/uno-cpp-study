#include <ConsoleCore/BaseScene/BaseScene.h>
#include <ConsoleCore/UserInterface/UserOptionData.h>

UserOptionData::UserOptionData(
    const std::string& desc, 
    std::function<std::shared_ptr<BaseScene>()> actionFunc)
    : 
    description(desc), 
    action(std::move(actionFunc)) {}

std::string UserOptionData::GetDescription() const {
    return description;
}

std::shared_ptr<BaseScene> UserOptionData::Execute() const {
    if (action)
        return action();
    return nullptr;
}
