#include "ConsoleCore/UserInterface/UserOptionData.h"

UserOptionData::UserOptionData(
    const std::string& desc, 
    std::function<void()> actionFunc)
    : 
    action { actionFunc },
    description(desc)
{

}

std::string UserOptionData::GetDescription() const {
    return description;
}

void UserOptionData::Execute() {
    if (action)
        action();
}