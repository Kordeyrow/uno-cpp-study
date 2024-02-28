#include <iostream>
#include <sstream>
#include "Uno/Scenes/GameplayScene.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define CLAMP(x, upper, lower) (MIN(upper, MAX(x, lower)))

#define SIMPLE_FUNC_REF(callable, arg) [&]() { callable(arg); }
//#define FuncRef(func) std::bind(&decltype(*this)::func, this)
#define MEMBER_FUNC_REF(func) std::bind(&std::remove_reference<decltype(*this)>::type::func, this)

#define ARROW_KEY (char)224


GameplayScene::GameplayScene() 
{
    duelists.emplace_back(std::make_shared<Duelist>("Player", duelistInitialHandSize));
    duelists.emplace_back(std::make_shared<Duelist>("Duelist 1", duelistInitialHandSize));
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
            MEMBER_FUNC_REF(Play)),
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
                MEMBER_FUNC_REF(BackFromSetPlayersNames))
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
        duelists.emplace_back(std::make_shared<Duelist>("Duelist " + std::to_string(duelists.size() - 1) , duelistInitialHandSize));
}

void GameplayScene::DecreaseTotalDuelists() {
    if (duelists.size() > minDuelists)
        duelists.erase(duelists.begin() + duelists.size() - 1);
}



#include <iostream>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES 
#include <math.h>

// Function to print a single duelist at a given position and their hand of cards.
void DrawDuelist(const Duelist& duelist, int x, int y, std::vector<std::string>& asciiTable) {
    int tableWidth = asciiTable[0].size();
    int tableHeight = asciiTable.size();

    // Generate the player's label with their name
    std::string playerLabel = "[" + duelist.name + "]";

    // Calculate the starting position for the player's label
    int labelStartX = x - playerLabel.length() / 2;

    // Make sure the label is within the bounds of the table
    if (labelStartX < 0) labelStartX = 0;
    if (labelStartX >= tableWidth - playerLabel.length()) labelStartX = tableWidth - playerLabel.length() - 1;

    // Print the player's label at the calculated position
    asciiTable[y].replace(labelStartX, playerLabel.length(), playerLabel);

    // Get the player's hand and print it as '*' characters
    std::string handString = duelist.hand->PrintHand();

    // Calculate the starting position for the player's hand
    int handStartX = x - handString.length() / 2;

    // Make sure the hand is within the bounds of the table
    if (handStartX < 0) handStartX = 0;
    if (handStartX >= tableWidth - handString.length()) handStartX = tableWidth - handString.length() - 1;

    // Print the player's hand at the calculated position
    asciiTable[y + 1].replace(handStartX, handString.length(), handString);
}

// Function to print the top card of the discard deck.
void printTopCard(int centerX, int centerY, std::vector<std::string>& asciiTable) {
    std::vector<std::string> topCard = {
        " ----- ",
        "|     |",
        "|Card |",
        "|     |",
        " ----- "
    };

    int cardWidth = topCard[0].length(); // Width of the card representation
    int cardHeight = topCard.size(); // Height of the card representation
    int startX = centerX - cardWidth / 2;
    int startY = centerY - cardHeight / 2;

    for (int i = 0; i < cardHeight; ++i) {
        for (int j = 0; j < cardWidth; ++j) {
            if ((startY + i) >= 0 && (startY + i) < asciiTable.size() &&
                (startX + j) >= 0 && (startX + j) < asciiTable[0].size()) {
                asciiTable[startY + i][startX + j] = topCard[i][j];
            }
        }
    }
}

bool IsStringEmpty(const std::string& s) {
    return s.find_first_not_of(' ') == std::string::npos;
}

void DrawLine(const std::string& s = "") {
    std::cout << s << std::endl;
}
void DrawLine(int count) {
    for (int i = 0; i < count; i++)
    {
        DrawLine();
    }
}







void GameplayScene::DrawTable(UserInterface* ui) {
    //while (true) {
        //int totalPlayers;
        //std::cout << "Enter the total number of players (up to 12): ";
        //std::cin >> totalPlayers;
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
        //system("CLS");

        //if (totalPlayers < 2 || totalPlayers > 12) {
        //    std::cout << "Number of players must be between 2 and 12." << std::endl;
        //    continue;
        //}

        const int tableWidth = 80;
        const int tableHeight = 32;
        std::vector<std::string> asciiTable(tableHeight, std::string(tableWidth, ' '));

        int centerX = tableWidth / 2;
        int centerY = tableHeight / 2;

        double startAngle = 3 * M_PI / 2;
         
        int totalPlayers = duelists.size();

        double radiusScaleFactor = 0.5 + 0.04 * (duelists.size() - 1);
        int radiusX = static_cast<int>((centerX - 10) * radiusScaleFactor);
        int radiusY = static_cast<int>((centerY - 5) * radiusScaleFactor * 2);

        for (int i = 0; i < totalPlayers; ++i) {
            double angle = startAngle - (2 * M_PI * i / totalPlayers);
            if (angle < 0) angle += 2 * M_PI;

            int x = static_cast<int>(centerX + radiusX * cos(angle));
            int y = static_cast<int>(centerY - (radiusY * sin(angle) / 2));

            //Duelist player("Player " + std::to_string(i + 1), 7);
            DrawDuelist(*duelists[i], x, y, asciiTable);
        }

        printTopCard(centerX, centerY, asciiTable);

        // Determine the first and last rows with content
        int firstRowWithContent = asciiTable.size(), lastRowWithContent = 0;
        for (int i = 0; i < asciiTable.size(); ++i) {
            if (!IsStringEmpty(asciiTable[i])) {
                firstRowWithContent = std::min(firstRowWithContent, i);
                lastRowWithContent = i;
            }
        }

        //DrawLine();

        std::stringstream screenData;
        screenData << std::endl;
        for (int i = firstRowWithContent; i <= lastRowWithContent; ++i) {
            screenData << asciiTable[i] << std::endl;
        }
        screenData << std::endl;
        //DrawLine();

        ui->SetScene(screenData.str());
    //}
}








void GameplayScene::Play()
{
    // MatchUI
    //
    UserInterface matchUI = *userInterface; // Keep state (only change copy)

    // use_card_func
    //
    auto use_card_func = [](Card& card)
    {
        std::cout << card.ColoredDescription() << "\n";
    };

    // Init Duelists card
    //
    for (size_t duelist_index = 0; duelist_index < duelists.size(); duelist_index++)
    {
        // Give total (duelistInitialHandSize) cards 
        //  for each duelist, from shuffled matchDeck
        //
        for (size_t card_count = 0; card_count < duelistInitialHandSize; card_count++)
        {
            Card& topCard = matchDeck.back();
            duelists[duelist_index]->hand->deck.push_back(topCard);
            matchDeck.pop_back();
        }
    }

    // Print MatchDeck
    //
    /*for (Card& card : matchDeck) {
        std::cout << card.ColoredDescription() << std::endl;
    }
    _getch();*/

    int lastOptionIndex = 0;

    // Turn Manager 
    //
    while (true) {

        // Draw
        //
        DrawTable(&matchUI);

        // Options (player cards)
        //
        matchUI.ClearOptions();
        auto& player = duelists[0];
        auto& player_deck = player->hand->deck;
        for (size_t i = 0; i < player_deck.size(); i++)
        {
            auto& card = player_deck[i];
            matchUI.AddUserOptions({
                   std::make_shared<UserOptionData>(
                   card.ColoredDescription(),
                   SIMPLE_FUNC_REF(use_card_func, card))
                });
        }

        // As options reset, selection was lost
        //
        matchUI.currentSelectedIndex = lastOptionIndex;

        matchUI.ShowOptions();

        // Action
        //
        matchUI.ReadOptionAndExecute();

        lastOptionIndex = matchUI.currentSelectedIndex;

        // Duelists names
        //
        /*std::vector<std::shared_ptr<UserOptionData>> options;
        int previousSelectedIndex = matchUI.currentSelectedIndex;
        matchUI.ClearOptions();
        for (size_t i = 0; i < duelists.size(); i++) {
            auto option = std::make_shared<UserOptionData>(duelists[i]->name);
            options.emplace_back(option);
        }
        matchUI.AddUserOptions(options);
        matchUI.AddUserOptions({
                std::make_shared<UserOptionData>(
                "Back",
                FUNC_REF(BackFromSetPlayersNames))
            });
        matchUI.currentSelectedIndex = previousSelectedIndex;*/
        //matchUI.ShowOptions();

        // Input
        //
        //auto input = matchUI.GetChar();

        // Move option selection
        //
        /*if (matchUI.ArrowMoveSelection(input))
            continue;*/

        // Read other options (not duelist names)
        //
       /* if (matchUI.currentSelectedIndex >= options.size()) {
            matchUI.ReadOptionAndExecute(input);
            if (endSetPlayers) {
                endSetPlayers = false;
                return;
            }
            continue;
        }

        matchUI.ReadOptionAndExecute();*/

    }
}
