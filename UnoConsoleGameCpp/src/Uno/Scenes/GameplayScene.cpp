#include <iostream>
#include <sstream>
#include "Uno/Scenes/GameplayScene.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define CLAMP(x, upper, lower) (MIN(upper, MAX(x, lower)))

//#define SIMPLE_FUNC_REF(callable, arg) [&]() { callable(arg); }
//#define SIMPLE_FUNC_REF(callable, ...) [__VA_ARGS__]() mutable { callable(__VA_ARGS__); }
//#define SIMPLE_FUNC_REF(callable, ...) [&]() mutable { callable(__VA_ARGS__); }
//#define SIMPLE_FUNC_REF(callable, ...) [&, __VA_ARGS__]() mutable { callable(__VA_ARGS__); }



#define SIMPLE_FUNC_REF(callable, ref, ref2, val, val2) [&, val]() mutable { callable(ref, ref2, val, val2); }

//#define SIMPLE_FUNC_REF(callable, ref, ...)) [&, __VA_ARGS__]() mutable { callable(ref, __VA_ARGS__); }
//#define SIMPLE_FUNC_REF(callable, ref, ...) [&ref, &](){ callable(ref, __VA_ARGS__); }
//#define SIMPLE_FUNC_REF(callable, ref, ...) [&ref, &]() mutable { callable(ref, __VA_ARGS__); }

//#define SIMPLE_FUNC_REF(callable, ref, ...) [&ref, &]() mutable { callable(ref, __VA_ARGS__); }
//#define SIMPLE_FUNC_REF(callable, ref, ...) [&ref, &]() mutable { callable(ref, __VA_ARGS__); }
//#define SIMPLE_FUNC_REF(callable, ref, ...) [&ref, &]() mutable { callable(ref, __VA_ARGS__); }



// working
//#define SIMPLE_FUNC_REF(callable, arg1, val) [&, val]() mutable { callable(arg1, val); }

//#define SIMPLE_FUNC_REF(callable, deck, index) [&, indexCopy = index]() mutable { callable(deck, indexCopy); }




//#define FuncRef(func) std::bind(&decltype(*this)::func, this)
//#define MEMBER_FUNC_REF(func) std::bind(&std::remove_reference<decltype(*this)>::type::func, this)
#define MEMBER_FUNC_REF(func, ...) std::bind(&std::remove_reference<decltype(*this)>::type::func, this, __VA_ARGS__)
//#define MEMBER_FUNC_REF(func, ...) std::bind(&std::remove_reference<decltype(*this)>::type::func, this, std::ref(__VA_ARGS__)...)


#define ARROW_KEY (char)224


GameplayScene::GameplayScene() 
{
    duelists.emplace_back(std::make_shared<Duelist>("Player", duelistInitialHandSize));

    for (size_t i = 0; i < startDuelists -1; i++)
    {
        duelists.emplace_back(std::make_shared<Duelist>("Duelist " + std::to_string(i), duelistInitialHandSize));
    }
    //minDuelists
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
            "Set duelists names",
            std::bind(&GameplayScene::SetPlayersNames, this)),

        std::make_shared<UserOptionData>(
            "Play",
            MEMBER_FUNC_REF(PlayMatch)),
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
#include <thread>



void GameplayScene::DrawDuelist(const Duelist& duelist, int colIndex, int rowIndex, std::vector<std::string>& asciiTable, bool highlight) {
    int tableWidth = asciiTable[0].size();
    int tableHeight = asciiTable.size();

    // ANSI escape code for yellow foreground
    std::string yellowForeground = "\033[33m";
    // ANSI escape code to reset styling
    std::string resetStyle = "\033[0m";

    std::string playerLabel = "[" + duelist.name + "]";
    int playerLabelSize = playerLabel.length();;

    // Generate the player's label with their name
    playerLabel = highlight ? yellowForeground + playerLabel + resetStyle : playerLabel;

    // Calculate the starting position for the player's label
    int labelStartX = colIndex - playerLabelSize / 2;

    // Make sure the label is within the bounds of the table
    if (labelStartX < 0) labelStartX = 0;
    if (labelStartX + playerLabelSize > tableWidth) labelStartX = tableWidth - playerLabelSize;

    // count zeroWidthChars
    int zeroWidthCharCount = 0;
    for (auto& zeroWidthCharactersInRow : zeroWidthCharactersByRow) {
        if (zeroWidthCharactersInRow.rowIndex == rowIndex && zeroWidthCharactersInRow.colIndex < colIndex) {
            zeroWidthCharCount += zeroWidthCharactersInRow.count;
        }
    }

    // Print the player's label at the calculated position
    asciiTable[rowIndex].replace(labelStartX + zeroWidthCharCount, playerLabelSize, playerLabel);

    // Get the player's hand and print it as '*' characters
    std::string handString = duelist.hand->PrintHand();
    int handStringSize = handString.length();
    if (highlight)
        handString = yellowForeground + handString + resetStyle;

    // Calculate the starting position for the player's hand
    int handStartX = colIndex - handStringSize / 2;

    // Make sure the hand is within the bounds of the table
    if (handStartX < 0) handStartX = 0;
    if (handStartX + handStringSize > tableWidth) handStartX = tableWidth - handStringSize;


    // count zeroWidthChars
    zeroWidthCharCount = 0;
    for (auto& zeroWidthCharactersInRow : zeroWidthCharactersByRow) {
        if (zeroWidthCharactersInRow.rowIndex == (rowIndex + 1) && zeroWidthCharactersInRow.colIndex < colIndex) {
            zeroWidthCharCount += zeroWidthCharactersInRow.count;
        }
    }
    // Print the player's hand at the calculated position
    asciiTable[rowIndex + 1].replace(handStartX + zeroWidthCharCount, handStringSize, handString);

    if (highlight) {
        //// ZeroWidthChar
        //
        // label
        //
        int colIndex = labelStartX + zeroWidthCharCount + playerLabelSize;
        zeroWidthCharactersByRow.push_back(ZeroWidthCharInfo{ rowIndex, colIndex, 9 });
        //
        // handString
        //
        colIndex = handStartX + zeroWidthCharCount + handStringSize;
        zeroWidthCharactersByRow.push_back(ZeroWidthCharInfo{ rowIndex + 1, colIndex, 9 });
    }
}

// Function to print the top card of the discard deck.
void GameplayScene::PrintTopCard(int centerX, int centerY, std::vector<std::string>& screenBuffer) {

    if (discardDeck.size() == 0) {
        return;
    }

    int var = 1;
    int descriptionSize = discardDeck.back().RawDescription().size();
    int spaceCountLeft = (8 - descriptionSize) / 2;
    if (spaceCountLeft < 0)
        spaceCountLeft = 0;
    int spaceCountRight = spaceCountLeft + ((8 - descriptionSize) % 2) ;
    if (spaceCountRight < 0)
        spaceCountRight = 0;

    std::string spacesLeft = std::string(spaceCountLeft, ' ');
    std::string spacesRight = std::string(spaceCountRight, ' ');

    std::vector<std::string> cardDrawTable = {
        " -------- ",
        "|        |",
        "|        |",
        "|" + spacesLeft + discardDeck.back().ColoredDescription() + spacesRight + "|",
        "|        |",
        "|        |",
        " -------- "
    };

    int totalCardRows = cardDrawTable.size(); // Height of the card representation
    int startY = centerY - totalCardRows / 2;
    int startX = centerX - cardDrawTable[0].length() / 2;

    for (int cardRowIndex = 0; cardRowIndex < totalCardRows; cardRowIndex++) {

        auto cardRow = cardDrawTable[cardRowIndex];
        //int lineWidth = cardRow.length();

        // ZeroWidthChar
        //
        bool beginZeroWidthCharCount = false;
        int zeroWidthCharCount = 0;

        auto totalCardCols = cardRow.size();
        int rowIndex = startY + cardRowIndex;
        for (int cardColIndex = 0; cardColIndex < totalCardCols; cardColIndex++) {

            int tableRowCount = screenBuffer.size();
            int tableColCount = screenBuffer[0].size();

            if ((rowIndex) >= 0 && (rowIndex) < tableRowCount &&
                (startX + cardColIndex) >= 0 && (startX + cardColIndex) < tableColCount) {

                auto& cardChar = cardRow[cardColIndex];

                screenBuffer[rowIndex][startX + cardColIndex] = cardChar;

                // ZeroWidthChar
                //
                if (cardChar == '\x1b')
                    beginZeroWidthCharCount = true;
                if (beginZeroWidthCharCount)
                    zeroWidthCharCount += 1;
                if (cardChar == 'm')
                    beginZeroWidthCharCount = false;
            }
        }

        // ZeroWidthChar
        //
        int colIndex = startX + totalCardCols;
        zeroWidthCharactersByRow.push_back(ZeroWidthCharInfo{ rowIndex, colIndex, zeroWidthCharCount });
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



void GameplayScene::DrawTable(UserInterface* ui, int duelist_index, bool winner) {
    
    const int tableWidth = 86;
    const int tableHeight = 32;
    std::vector<std::string> screenBuffer(tableHeight, std::string(tableWidth, ' '));
    zeroWidthCharactersByRow.clear();

    int centerX = tableWidth / 2;
    int centerY = tableHeight / 2;

    double startAngle = 3 * M_PI / 2;
         
    int totalPlayers = duelists.size();

    double radiusScaleFactor = 0.5 + 0.04 * (duelists.size() - 1);
    int radiusX = static_cast<int>((centerX - 10) * radiusScaleFactor);
    int radiusY = static_cast<int>((centerY - 5) * radiusScaleFactor * 2);

    PrintTopCard(centerX, centerY, screenBuffer);


    // ======    ======


    for (int i = 0; i < totalPlayers; ++i) {
        double angle = startAngle - (2 * M_PI * i / totalPlayers);
        if (angle < 0) angle += 2 * M_PI;

        int x = static_cast<int>(centerX + radiusX * cos(angle));
        int y = static_cast<int>(centerY - (radiusY * sin(angle) / 2));

        //Duelist player("Player " + std::to_string(i + 1), 7);
        DrawDuelist(*duelists[i], x, y, screenBuffer, i == duelist_index);
    }

    // Determine the first and last rows with content
    int firstRowWithContent = screenBuffer.size(), lastRowWithContent = 0;
    for (int i = 0; i < screenBuffer.size(); ++i) {
        if (!IsStringEmpty(screenBuffer[i])) {
            firstRowWithContent = std::min(firstRowWithContent, i);
            lastRowWithContent = i;
        }
    }


    // ======    ======
    

    // DrawLine();

    std::stringstream screenData;
    screenData << std::endl;
    for (int i = firstRowWithContent; i <= lastRowWithContent; ++i) {
        screenData << screenBuffer[i] << std::endl;
    }
    screenData << std::endl;
    //DrawLine();

    ui->SetScene(screenData.str());
}


void GameplayScene::DrawCard(std::vector<Card>& target) {
    if (drawDeck.size() == 0)
        return;
    Card card = std::move(drawDeck.back()); // Move the top card into 'card'
    drawDeck.pop_back(); // This is now safe; 'card' is no longer tied to the 'matchDeck'
    target.push_back(card);
}

void GameplayScene::PlayerOptionDrawCard() {

    if (cardsToBuy_2 > 0) {
        for (size_t i = 0; i < cardsToBuy_2; i++) {
            DrawCard(duelists[0]->hand->deck);
        }
        cardsToBuy_2 = 0;
    }
    else {
        DrawCard(duelists[0]->hand->deck);
    }

    playerTurnActionDone = true;
}


void GameplayScene::PlayMatch()
{
    // Reset game
    //
    discardDeck = {};
    cardsToBuy_2 = 0;
    cardsToBuy_4 = 0;
    skip = false;
    //discardColorID;
    endSetPlayers = false;
    dir = 1;
    playerTurnActionDone = false;
    duelistTurnActionDone = false;
    duelists = {};
    duelists.emplace_back(std::make_shared<Duelist>("Player", duelistInitialHandSize));
    for (size_t i = 0; i < startDuelists - 1; i++)
    {
        duelists.emplace_back(std::make_shared<Duelist>("Duelist " + std::to_string(i), duelistInitialHandSize));
    }
    drawDeck = ShuffleDeck(CreateMatchDeck());

    // winner
    winner = nullptr;

    // MatchUI
    //
    UserInterface matchUI = *userInterface; // Keep state (only change copy)


    // ======( )  use_card_func  ( )====== //
    //                                     
    //                                     
    auto* discardDeckRef = &discardDeck;
    auto* cardsToBuy_2Ref = &cardsToBuy_2;
    auto* skipRef = &skip;
    auto* duelistUsedCardRef = &duelistTurnActionDone;
    auto* playerUsedCardRef = &playerTurnActionDone;
    auto* dirRef = &dir;
    auto* winnerRef = &winner;

    auto use_card_func = 
        [discardDeckRef, duelistUsedCardRef, cardsToBuy_2Ref, skipRef, playerUsedCardRef, dirRef, winnerRef]
        (std::shared_ptr<Duelist> duelist, std::vector<Card>& container, int cardIndex, bool isPlayer)
    {
        if (discardDeckRef->size() == 0)
            return;

        // can use if has same color, or numbered card same number
        //
        auto& card = container[cardIndex];

        if (*cardsToBuy_2Ref > 0 && card.typeID != Card::DRAW_2) {
            return;
        }

        if (card.typeID == Card::NUMBERED) {
            if (card.number != discardDeckRef->back().number 
                && card.colorID != discardDeckRef->back().colorID) {
                return;
            }
        }

        if ((card.colorID == discardDeckRef->back().colorID)
            || (card.typeID == discardDeckRef->back().typeID)) {
            
            if (card.typeID == Card::SKIP) {
                *skipRef = true;
            }

            if (card.typeID == Card::DRAW_2) {
                *cardsToBuy_2Ref += 2;
            }

            if (card.typeID == Card::REVERSE) {
                *dirRef *= -1;
            }

            // add to discardDeck
            //
            discardDeckRef->push_back(card);

            // remove from play deck1
            //
            container.erase(container.begin() + cardIndex);

            *duelistUsedCardRef = true;
            if (isPlayer)
                *playerUsedCardRef = true;
        }

        if (container.size() == 0) {
            *winnerRef = duelist;
        }
    };


    // ======( )  Init Duelists deck  ( )======
    //                                     
    //                                     
    for (size_t duelist_index = 0; duelist_index < duelists.size(); duelist_index++)
    {
        // Give total (duelistInitialHandSize) cards 
        //  for each duelist, from shuffled matchDeck
        //
        for (size_t card_count = 0; card_count < duelistInitialHandSize; card_count++)
        {
            DrawCard(duelists[duelist_index]->hand->deck);
        }
    }

    DrawCard(discardDeck);

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

        // Options (player cards)
        //
        matchUI.ClearOptions();
        auto& duelist = duelists[0];
        auto& duelist_deck = duelist->hand->deck;
        for (size_t i = 0; i < duelist_deck.size(); i++)
        {
            auto& card = duelist_deck[i];
            matchUI.AddUserOptions({
                   std::make_shared<UserOptionData>(
                       card.ColoredDescription(),
                       SIMPLE_FUNC_REF(use_card_func, duelist, duelist_deck, i, true)
                   )
                });
        }

        matchUI.AddUserOptions({
               std::make_shared<UserOptionData>(
                   "Draw Card",
                   MEMBER_FUNC_REF(PlayerOptionDrawCard)
               )
            });

        // As options reset, selection was lost
        //
        matchUI.currentSelectedIndex = lastOptionIndex;
        matchUI.ShowOptions();

        if (playerTurnActionDone) {

            playerTurnActionDone = false;
            duelistTurnActionDone = false;
            // AI duelist turns
            //
            int currentDuelistIndex = 0;

            if (dir == 1)
                currentDuelistIndex = 1;
            if (dir == -1)
                currentDuelistIndex = duelists.size() - 1;

            // while not player turn
            //
            while (currentDuelistIndex != 0) {

                // delay
                //
                DrawTable(&matchUI, currentDuelistIndex, false);

                if (skip) {
                    skip = false;
                    currentDuelistIndex += dir;
                    if (currentDuelistIndex >= duelists.size())
                        currentDuelistIndex = 0;
                    continue;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(turnActionDelay));

                // Give total (duelistInitialHandSize) cards 
                //  for each duelist, from shuffled matchDeck
                //
                auto& deck = duelists[currentDuelistIndex]->hand->deck;

                // can use if has same color]
                //                
                for (size_t card_index = 0; card_index < deck.size(); card_index++)
                {
                    use_card_func(duelists[currentDuelistIndex], deck, card_index, false);

                    if (duelistTurnActionDone) {
                        break;
                    }
                }

                if (winner != nullptr) {
                    winnerIndex = currentDuelistIndex;
                    break;
                }

                if (duelistTurnActionDone) {
                    duelistTurnActionDone = false;
                }
                else {
                    if (cardsToBuy_2 > 0) {
                        for (size_t i = 0; i < cardsToBuy_2; i++) {
                            DrawCard(duelists[currentDuelistIndex]->hand->deck);
                        }
                        cardsToBuy_2 = 0;
                    }
                    else {
                        DrawCard(duelists[currentDuelistIndex]->hand->deck);
                    }
                }

                currentDuelistIndex += dir;
                if (currentDuelistIndex >= duelists.size())
                    currentDuelistIndex = 0;
            }
        }


        DrawTable(&matchUI, 0, false);
        
        if (winner == nullptr) {
            // Player Action
            //
            if (skip) {
                skip = false;
                playerTurnActionDone = true;
            }
            else {
                matchUI.ReadOptionAndExecute();
                lastOptionIndex = matchUI.currentSelectedIndex;
            }
            if (winner)
                winnerIndex = 0;
        }

        if (winner != nullptr) {
            matchUI.SetUserOptions({});
            DrawTable(&matchUI, 0, true);
            _getch();
            break;
        }


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
