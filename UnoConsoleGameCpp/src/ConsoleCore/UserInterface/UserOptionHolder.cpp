#include "ConsoleCore/UserInterface/UserOptionHolder.h"

auto UserOptionHolder::GetUserOptionDataList() -> std::vector<std::shared_ptr<UserOptionData>> const {
    return userOptionDataList;
};