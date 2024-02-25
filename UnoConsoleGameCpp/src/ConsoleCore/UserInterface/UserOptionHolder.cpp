#include "ConsoleCore/UserInterface/UserOptionHolder.h"

auto UserOptionHolder::GetUserActions() -> std::vector<std::shared_ptr<UserOptionData>> const {
    return userOptionDataList;
};