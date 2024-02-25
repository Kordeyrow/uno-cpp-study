#pragma once
#include <string>
#include <functional>

class UserOptionData {
public:
    UserOptionData(
        const std::string& desc, 
        std::function<void()> actionFunc);

    std::string GetDescription() const;
    void Execute();
private:
    std::string description;
    std::function<void()> action;
};