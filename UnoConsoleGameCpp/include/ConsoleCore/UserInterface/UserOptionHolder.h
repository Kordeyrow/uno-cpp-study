#pragma once
#include <vector>
#include <memory>
#include "UserOptionData.h"

class UserOptionHolder {
public:
    auto GetUserActions() -> std::vector<std::shared_ptr<UserOptionData>> const;
protected:
    std::vector<std::shared_ptr<UserOptionData>> userOptionDataList;
};
