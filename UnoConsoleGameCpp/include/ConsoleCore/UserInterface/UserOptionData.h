#pragma once
#include <string>
#include <functional>

class UserOptionData {
public:
    UserOptionData(
        const std::string& desc, 
        std::function<void()> actionFunc = nullptr);

    std::string GetDescription() const;
    void SetDescription(const std::string& desc);
    void Execute();
private:
    std::string description;
    std::function<void()> action;
};