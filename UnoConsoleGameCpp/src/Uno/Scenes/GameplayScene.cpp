#include <iostream>
#include "Uno/Scenes/GameplayScene.h"

//#define FuncRef(func) std::bind(&decltype(*this)::func, this)
#define FUNC_REF(func) std::bind(&std::remove_reference<decltype(*this)>::type::func, this)

#define ARROW_KEY (char)224


GameplayScene::GameplayScene() 
{
    duelists.emplace_back(std::make_shared<Duelist>("Duelist 1"));
    duelists.emplace_back(std::make_shared<Duelist>("Duelist 2"));
}

auto GameplayScene::Init() -> void
{
    userInterface->SetTitle("--[ GAMEPLAY ]--");
    userInterface->AddUserOptions({

        std::make_shared<UserOptionData>(
            "Increase total duelists",
            std::bind(&GameplayScene::IncreaseTotalDuelists, this)),

        std::make_shared<UserOptionData>(
            "Decrease total duelists",
            std::bind(&GameplayScene::DecreaseTotalDuelists, this)),

        std::make_shared<UserOptionData>(
            "Set players names",
            std::bind(&GameplayScene::SetPlayersNames, this)),

        std::make_shared<UserOptionData>(
            "Play",
            FUNC_REF(Play)),
    });
}

auto GameplayScene::Run() -> std::shared_ptr<BaseScene>
{
    userInterface->SetScene("totalDuelists = " + std::to_string(duelists.size()));
    userInterface->ReadOptionAndExecute();
    return shared_from_this();
}

void GameplayScene::BackFromSetPlayersNames() {
    endSetPlayers = true;
}

void GameplayScene::SetPlayersNames() {

    userInterface->SetScene("");

    // Keep state (only change copy)
    //
    auto uiCopy = *userInterface;

    // Setup players naming UI
    //
    UserInterface playersNamingUI = uiCopy;

    //playersNamingUI.SetScene("totalDuelists = " + std::to_string(duelists.size()));

    playersNamingUI.currentSelectedIndex = 0;

    while (true) {

        // Options
        //
        std::vector<std::shared_ptr<UserOptionData>> options;
        int previousSelectedIndex = playersNamingUI.currentSelectedIndex;
        playersNamingUI.ClearOptions();
        for (size_t i = 0; i < duelists.size(); i++) {
            auto option = std::make_shared<UserOptionData>(duelists[i]->name);
            options.emplace_back(option);
        }
        playersNamingUI.AddUserOptions(options);
        playersNamingUI.AddUserOptions({
                std::make_shared<UserOptionData>(
                "Back",
                FUNC_REF(BackFromSetPlayersNames))
            });
        playersNamingUI.currentSelectedIndex = previousSelectedIndex;
        playersNamingUI.ShowOptions();
    
        // Input
        //
        auto input = playersNamingUI.GetChar();

        // Move selection
        //
        if (playersNamingUI.ArrowMoveSelection(input))
            continue;

        // Other options (not name)
        //
        if (playersNamingUI.currentSelectedIndex >= options.size()) {
            playersNamingUI.ReadOptionAndExecute(input);
            if (endSetPlayers) {
                endSetPlayers = false;
                return;
            }
            continue;
        }
        
        // Erase letter
        //
        auto index = playersNamingUI.currentSelectedIndex;
        auto& duelistName = duelists[index]->name;
        if ((int)input == 8) {
            if (duelistName.size() > 0) {
                duelistName.pop_back();
                options[index]->SetDescription(duelistName);
                continue;
            }
        }

        // Add letter
        //
        if (std::isalpha((unsigned char)input)) {           
            duelistName.push_back((unsigned char)input);
            options[index]->SetDescription(duelistName);
            continue;
        }
    }
}

void GameplayScene::IncreaseTotalDuelists() {
    if (duelists.size() < maxDuelists)
        duelists.emplace_back(std::make_shared<Duelist>("Duelist " + std::to_string(duelists.size())));
}

void GameplayScene::DecreaseTotalDuelists() {
    if (duelists.size() > minDuelists)
        duelists.erase(duelists.begin() + duelists.size() - 1);
}

void GameplayScene::Play()
{
}
