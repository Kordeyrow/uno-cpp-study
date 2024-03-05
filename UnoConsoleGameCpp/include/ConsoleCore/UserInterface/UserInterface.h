#pragma once
#include <map>
#include <vector>
#include "UserOptionData.h"
#include "UserOption.h"

class UserInterface {
public:
    UserInterface() = default;
    void AddUserOptions(std::vector<std::shared_ptr<UserOptionData>> option);
    void ClearOptions();
    void ShowOptions(int selectedIndex = -1, bool chosen = false);
    void ReadOptionAndExecute(char input = -1, bool thread = false);
    bool ArrowMoveSelection(char input);

    char GetChar();

    UserInterface GetState();
    void SetState(UserInterface stateCopy);

    // Copy constructor
    UserInterface(const UserInterface& other)
        : currentSelectedIndex(other.currentSelectedIndex),
        title(other.title), scene(other.scene),
        userOptions(other.userOptions), userMessage(other.userMessage) {

        // Clone each UserOption in the map
        for (const auto& pair : other.optionFromKey) {
            optionFromKey[pair.first] = pair.second->Clone();
        }
    }

    auto Draw(int indexStart = 0) -> void;

    auto SetTitle(const std::string& title) -> void;
    auto SetScene(const std::string& scene) -> void;
    auto SetUserOptions(const std::string& userOptions) -> void;
    auto SetUserMessage(const std::string& userMessage) -> void;

    auto AddTitle(const std::string& title) -> void;
    auto AddScene(const std::string& scene) -> void;
    auto AddUserOption(const std::string& userOptions) -> void;
    auto AddUserMessage(const std::string& userMessage) -> void;

    int currentSelectedIndex = 0;

private:
    std::map<char, std::unique_ptr<UserOption>> optionFromKey;
    auto ReadInputLine() -> std::string const;
    auto ReadInputKey() -> char const;
    auto ClearConsole() -> void const;

    std::string title = "";
    std::string scene = "";
    std::string userOptions = "";
    std::string userMessage = "";
};