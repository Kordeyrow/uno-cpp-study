#include "ConsoleCore/UserInterface/UserOptionData.h"

UserOptionData::UserOptionData(
    const std::string& desc, 
    std::function<void()> actionFunc)
    : 
    action { actionFunc },
    description( std::string(desc) )
{

}

std::string UserOptionData::GetDescription() const {
    return description;
}

void UserOptionData::SetDescription(const std::string& desc)
{
    description = desc;
}

void UserOptionData::Execute() {
    if (action)
        action();
}