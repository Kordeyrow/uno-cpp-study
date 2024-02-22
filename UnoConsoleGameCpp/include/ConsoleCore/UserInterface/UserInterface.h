#pragma once
#include <map>
#include <vector>
#include "UserOptionData.h"
#include "UserOption.h"

class UserInterface {
public:
    UserInterface() = default;
    void AddUserOption(const std::vector<std::shared_ptr<UserOptionData>> option);
    void ClearOptions();
    void ShowOptions(int selectedIndex, bool chosen);
    std::shared_ptr<BaseScene> ReadOptionAndExecute();

    auto Draw() -> void;

    auto SetTitle(const std::string& title) -> void;
    auto SetScene(const std::string& scene) -> void;
    auto SetUserOptions(const std::string& userOptions) -> void;
    auto SetUserMessage(const std::string& userMessage) -> void;

    auto AddTitle(const std::string& title) -> void;
    auto AddScene(const std::string& scene) -> void;
    auto AddUserOption(const std::string& userOptions) -> void;
    auto AddUserMessage(const std::string& userMessage) -> void;
private:
    int currentSelectedIndex = 0;
    std::map<char, std::unique_ptr<UserOption>> optionFromKey;
    auto ReadInputLine() -> std::string const;
    auto ReadInputKey() -> char const;
    auto ClearConsole() -> void const;

    std::string title = "";
    std::string scene = "";
    std::string userOptions = "";
    std::string userMessage = "";
};
