#include <iostream>
#include <sstream>
#include "Uno/Scenes/GameplayScene.h"
#include <chrono>
#include <numeric> // For std::iota

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define CLAMP(x, upper, lower) (MIN(upper, MAX(x, lower)))

#define SIMPLE_VOID_FUNC_REF(callable, arg) [&]() { callable(arg); }
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
        duelists.emplace_back(std::make_shared<Duelist>("Duelist " + std::to_string(i+2), duelistInitialHandSize));
    }
    //minDuelists
}

auto GameplayScene::Init() -> void
{
    userInterface->SetTitle("--[ GAMEPLAY ]--");
    userInterface->AddUserOptions({

        std::make_shared<UserOptionData>(
            "Add new Duelist",
            std::bind(&GameplayScene::IncreaseTotalDuelists, this)),

        std::make_shared<UserOptionData>(
            "Remove last Duelist",
            std::bind(&GameplayScene::DecreaseTotalDuelists, this)),

        std::make_shared<UserOptionData>(
            "Update Duelists names",
            std::bind(&GameplayScene::SetPlayersNames, this)),

        std::make_shared<UserOptionData>(
            "Play",
            MEMBER_FUNC_REF(PlayMatch)),
    });
}

auto GameplayScene::Run() -> std::shared_ptr<BaseScene>
{
    userInterface->SetScene("Total Duelists = " + std::to_string(duelists.size()));
    userInterface->ReadOptionAndExecute();
    return shared_from_this();
}

void GameplayScene::BackFromSetPlayersNames() {
    endSetPlayers = true;
}

void GameplayScene::SetPlayersNames() {

    userInterface->SetScene("");
    //userInterface->

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
        duelists.emplace_back(std::make_shared<Duelist>("Duelist " + std::to_string(duelists.size()+1) , duelistInitialHandSize));
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



void GameplayScene::DrawDuelist(
    const Duelist& duelist, 
    int colIndex, 
    int rowIndex, 
    std::vector<std::string>& asciiTable, 
    bool highlight,
    bool winner,
    bool gameover) 
{
    int tableWidth = asciiTable[0].size();
    int tableHeight = asciiTable.size();

    // ANSI escape code for yellow foreground

    std::string duelistColor = "\033[33m"; // yellow
    if (gameover)
        duelistColor = "\033[31m"; // red (loser)
    if (winner)
        duelistColor = "\033[32m"; // green (winner)

    // ANSI escape code to reset styling
    std::string resetStyle = "\033[0m";

    std::string playerLabel = "[" + duelist.name + "]";
    int playerLabelSize = playerLabel.length();;

    // Generate the player's label with their name
    playerLabel = highlight ? duelistColor + playerLabel + resetStyle : playerLabel;

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
        handString = duelistColor + handString + resetStyle;

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

    auto descriptionTextArea = 11 - 2; // "|         |" - "|" "|"

    int var = 1;
    int descriptionSize = discardDeck.back().RawDescription().size();
    int spaceCountLeft = (descriptionTextArea - descriptionSize) / 2;
    if (spaceCountLeft < 0)
        spaceCountLeft = 0;
    int spaceCountRight = spaceCountLeft + ((descriptionTextArea - descriptionSize) % 2) ;
    if (spaceCountRight < 0)
        spaceCountRight = 0;

    std::string spacesLeft = std::string(spaceCountLeft, ' ');
    std::string spacesRight = std::string(spaceCountRight, ' ');

    std::vector<std::string> cardDrawTable = {
        " --------- ",
        "|         |",
        "|         |",
        "|" + spacesLeft + discardDeck.back().ColoredDescription() + spacesRight + "|",
        "|         |",
        "|         |",
        " --------- "
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



void GameplayScene::DrawTable(UserInterface* ui, int current_duelist_index, bool winner) {
    
    const int tableWidth = 86;
    const int tableHeight = 32;
    std::vector<std::string> pixelsTable(tableHeight, std::string(tableWidth, ' '));
    zeroWidthCharactersByRow.clear();

    // Find center
    //
    int centerX = tableWidth / 2;
    int centerY = tableHeight / 2;

    // Calculate radius X Y 
    //
    double startAngle = 3 * M_PI / 2;         
    int totalDuelists = duelists.size();
    double radiusScaleFactor = 0.5 + 0.04 * (totalDuelists - 1);
    int radiusX = (centerX - 10) * radiusScaleFactor;
    int radiusY = (centerY - 5) * radiusScaleFactor * 2;

    // Print
    //
    PrintTopCard(centerX, centerY, pixelsTable);
    
    // Calculate duelists posistion
    //
    for (int i = 0; i < totalDuelists; ++i) {

        double angle = startAngle - (2 * M_PI * i / totalDuelists);
        if (angle < 0) angle += 2 * M_PI;

        int x = static_cast<int>(centerX + radiusX * cos(angle));
        int y = static_cast<int>(centerY - (radiusY * sin(angle) / 2));

        //Duelist player("Player " + std::to_string(i + 1), 7);
        auto gameIsOver = winner;
        auto duelistIsWinner = winner ? i == current_duelist_index : false;
        DrawDuelist(
            *duelists[i], 
            x, 
            y, 
            pixelsTable, 
            (i == current_duelist_index) || gameIsOver,
            duelistIsWinner,
            gameIsOver);
    }

    // Determine the first and last rows with content
    //
    int firstRowWithContent = pixelsTable.size(), lastRowWithContent = 0;
    for (int i = 0; i < pixelsTable.size(); ++i) {
        if (!IsStringEmpty(pixelsTable[i])) {
            firstRowWithContent = std::min(firstRowWithContent, i);
            lastRowWithContent = i;
        }
    }

    // Draw screen
    //
    std::stringstream screenData;
    screenData << std::endl;
    for (int i = firstRowWithContent; i <= lastRowWithContent; ++i) {
        screenData << pixelsTable[i] << std::endl;
    }
    screenData << std::endl;

    ui->SetScene(screenData.str());
}

void GameplayScene::WaitForAnyKeyOrDelay(UserInterface* matchUI) {

    // Step by step
    //
    //auto input = _getch();
    //if (input == (char)224) {
    //    // Move selected index
    //    //
    //    auto input = _getch();
    //}
    //while (_kbhit())
    //    _getch();
    //return;


    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    auto now = start;
    duration<double> elapsed_seconds;
    int elapsed_int_seconds = 0;
    //float waitingTimeSeconds = 1.1f; 

    while (true) {

        auto fetchedInput = _kbhit();

        if (fetchedInput) {

            if (matchUI) {
                matchUI->ReadOptionAndExecute(fetchedInput, true);
                matchUI->ShowOptions();
                //lastOptionIndex = matchUI->currentSelectedIndex;
                //DrawTable(&matchUI, 0, false);
                while (_kbhit())
                    _getch();
            }

            //break;
        }

        if (wildPlayed)
            break;

        now = high_resolution_clock::now();

        //elapsed_seconds = end - start;
        elapsed_seconds = duration_cast<duration<float>>(now - start);
        // If more than one second has passed
        if (elapsed_seconds.count() >= turnActionDelay) {
            break;
        }
    }
}

void WaitForAnyKey() {
    auto input = _getch();
    if (input == (char)224) {
        // Move selected index
        //
        auto input = _getch();
    }
    while (_kbhit())
        _getch();
}

void GameplayScene::DrawCard(std::vector<Card>& targetDeck) {

    if (drawDeck.size() == 0) {
        if (discardDeck.size() <= 1) {
            // not enough cards
            return;
        }

        // Temporarily hold the top card
        Card topCard = std::move(discardDeck.back());
        discardDeck.pop_back();

        // Move all remaining cards from discardDeck to drawDeck
        std::move(discardDeck.begin(), discardDeck.end(), std::back_inserter(drawDeck));
        discardDeck.clear(); // Clear the source deck

        ShuffleDeck(drawDeck);

        // Put the top card back on the discardDeck
        discardDeck.push_back(std::move(topCard));
    }

    if (drawDeck.empty()) {
        throw std::runtime_error("No cards left to draw after attempting to replenish");
    }

    Card card = std::move(drawDeck.back()); // Move the top card into 'card'
    drawDeck.pop_back(); // This is now safe; 'card' is no longer tied to the 'matchDeck'
    targetDeck.push_back(card);

    if (&targetDeck == &duelists[0]->hand->deck) {
        if (targetDeck.size() > 1 && playerSaidUno == true) {
            playerSaidUno == false;
        }
    }
}

void GameplayScene::PlayerOptionDrawCard() {

    if (playerTurn == false)
        return;

    auto& playerDeck = duelists[0]->hand->deck;

    if (cardsToBuy_2 == 0
        && cardsToBuy_4 == 0) {
        DrawCard(playerDeck);
    }
    else {
        if (cardsToBuy_4 > 0) {
            for (size_t i = 0; i < cardsToBuy_4; i++) {
                DrawCard(playerDeck);
            }
            cardsToBuy_4 = 0;
        }
        if (cardsToBuy_2 > 0) {
            for (size_t i = 0; i < cardsToBuy_2; i++) {
                DrawCard(playerDeck);
            }
            cardsToBuy_2 = 0;
        }
    }

    playerTurnActionDone = true;
}

void GameplayScene::SayUno() {

    if (playerTurn == false)
        return;

    playerSaidUno = true;
    publicSharedMatchUI->currentSelectedIndex = publicSharedMatchUI->currentSelectedIndex - 1;
}


void GameplayScene::PlayMatch()
{
    // Reset game
    //
    discardDeck = {};
    cardsToBuy_2 = 0;
    cardsToBuy_4 = 0;
    skip = false;
    endSetPlayers = false;
    dir = 1;
    playerTurnActionDone = false;
    duelistTurnActionDone = false;

    auto duelistsCopy = duelists;
    duelists = {};

    // Add Player (first duelist which is the player, will always be on table bottom)
    //
    duelists.emplace_back(std::make_shared<Duelist>(duelistsCopy[0]->name, duelistInitialHandSize));

    // Add adversaries
    //
    // Start from 1 to skip the player at index 0
    std::vector<int> indexes(duelistsCopy.size() - 1);
    std::iota(indexes.begin(), indexes.end(), 1); // Fill with 1, 2, 3, ..., n-1

    std::random_device rd;
    std::mt19937 g(rd());

    while (!indexes.empty()) {
        // Randomly pick an index from the vector of indexes
        std::uniform_int_distribution<> dis(0, indexes.size() - 1);
        int randomIndex = dis(g);
        int randomDuelistIndex = indexes[randomIndex];
        auto& randomDuelist = duelistsCopy[randomDuelistIndex];
        auto randomDuelistName = duelistsCopy[randomDuelistIndex]->name;

        // Use the picked index to add a duelist from duelistsCopy to duelists
        duelists.emplace_back(std::make_shared<Duelist>(randomDuelistName, duelistInitialHandSize));

        // Remove the used index from the indexes vector to avoid repetition
        indexes.erase(indexes.begin() + randomIndex);
    }


    drawDeck = ShuffleDeck(CreateMatchDeck());

    // winner
    winner = nullptr;

    // MatchUI
    //
    UserInterface matchUI = *userInterface; // Keep state (only change copy)
    publicSharedMatchUI = &matchUI;

    // ======( )  use_card_func  ( )====== //
    //                                     
    //                                     
    auto* discardDeckRef = &discardDeck;
    auto* cardsToBuy_2Ref = &cardsToBuy_2;
    auto* cardsToBuy_4Ref = &cardsToBuy_4;
    auto* skipRef = &skip;
    auto* duelistUsedCardRef = &duelistTurnActionDone;
    auto* playerUsedCardRef = &playerTurnActionDone;
    auto* dirRef = &dir;
    auto* winnerRef = &winner;
    auto* playerTurnRef = &playerTurn;
    auto* wildPlayedRef = &wildPlayed;
    auto* publicSharedMatchUIRef = &publicSharedMatchUI;

    auto use_card_func = 
        [discardDeckRef, duelistUsedCardRef, cardsToBuy_2Ref, cardsToBuy_4Ref, skipRef, playerUsedCardRef, dirRef, winnerRef, playerTurnRef, wildPlayedRef, publicSharedMatchUIRef]
        (std::shared_ptr<Duelist> duelist, std::vector<Card>& duelistDeck, int cardIndex, bool isPlayer)
    {

        if (discardDeckRef->size() == 0)
            return;


        auto& useTryCard = duelistDeck[cardIndex];


        // can use if has same color, or numbered card same number
        //
        if (*cardsToBuy_4Ref > 0
            && useTryCard.typeID != Card::WILD_DRAW_4) {
            return;
        }
        else {
            if (*cardsToBuy_2Ref > 0
                && useTryCard.typeID != Card::DRAW_2) {
                return;
            }
        }



        // can use if has same color, or numbered card same number
        //
        if (useTryCard.typeID == Card::NUMBERED) {
            if (useTryCard.number != discardDeckRef->back().number 
                && useTryCard.colorID != discardDeckRef->back().colorID
                && discardDeckRef->back().colorID != Card::ALL) {
                return;
            }
        }

        if ((discardDeckRef->back().colorID != Card::ALL && useTryCard.colorID != discardDeckRef->back().colorID)
            && (useTryCard.typeID != discardDeckRef->back().typeID)
            && useTryCard.typeID != Card::WILD_DRAW_4) {
            return;
        }

        if (isPlayer 
            && *playerTurnRef == false
            && useTryCard.typeID != Card::WILD_DRAW_4)
            return;


        //aaa

        if (useTryCard.typeID == Card::SKIP) {
            *skipRef = true;
        }

        if (useTryCard.typeID == Card::DRAW_2) {
            *cardsToBuy_2Ref += 2;
        }

        if (useTryCard.typeID == Card::REVERSE) {
            *dirRef *= -1;
        }

        if (useTryCard.typeID == Card::WILD_DRAW_4) {

            *cardsToBuy_4Ref += 4;

            if (isPlayer) {
                if (*playerTurnRef == false) {
                    *wildPlayedRef = true;
                }

                // CHOSE COLOR
                //
                //UserInterface matchUI = *userInterface; // Keep state (only change copy)
                UserInterface choseColorUI = **publicSharedMatchUIRef;

                int lastOptionIndex = 0;
                int color = 0;

                while (color == 0) {
                    choseColorUI.ClearOptions();
                    choseColorUI.AddUserOptions(
                        {
                            std::make_shared<UserOptionData>(
                                Card::DoColorBGText("  ", Card::RED),
                                [&color]() { color = Card::RED; }
                            ),
                            std::make_shared<UserOptionData>(
                                Card::DoColorBGText("  ", Card::YELLOW),
                                [&color]() { color = Card::YELLOW; }
                            ),
                            std::make_shared<UserOptionData>(
                                Card::DoColorBGText("  ", Card::BLUE),
                                [&color]() { color = Card::BLUE; }
                            ),
                            std::make_shared<UserOptionData>(
                                Card::DoColorBGText("  ", Card::GREEN),
                                [&color]() { color = Card::GREEN; }
                            ),
                        }
                    );
                    choseColorUI.ShowOptions();
                    choseColorUI.currentSelectedIndex = lastOptionIndex;
                    choseColorUI.ReadOptionAndExecute();
                    lastOptionIndex = choseColorUI.currentSelectedIndex;
                }
                useTryCard.colorID = color;
            }
            else {
                useTryCard.colorID = Card::GetRandomColorCode();
            }
        }

        // add to discardDeck
        //
        discardDeckRef->push_back(useTryCard);

        // remove from play deck1
        //
        duelistDeck.erase(duelistDeck.begin() + cardIndex);

        *duelistUsedCardRef = true;
        if (isPlayer)
            *playerUsedCardRef = true;

        if (duelistDeck.size() == 0) {
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
            /*if (duelist_index > 0) {
                DrawCard(duelists[duelist_index]->hand->deck);

            }*/
        }
    }

    int notDraw_4Count = 1;

    while (drawDeck.back().typeID != Card::NUMBERED) {
        DrawCard(discardDeck);
    }
    DrawCard(discardDeck);

    // Print MatchDeck
    //
    /*for (Card& card : matchDeck) {
        std::cout << card.ColoredDescription() << std::endl;
    }
    SafeGetChar();*/

    int lastOptionIndex = 0;

    // Turn Manager 
    //
    while (true) {

        playerTurn = false;

        // Options (player cards)
        //
        matchUI.ClearOptions();
        auto& player = duelists[0];
        auto& player_deck = player->hand->deck;

        if (duelists[0]->hand->deck.size() > 2)
            playerSaidUno = false;

        if (duelists[0]->hand->deck.size() == 1 && playerSaidUno == false && playerTurnActionDone == false) {

            matchUI.AddUserOptions({
                std::make_shared<UserOptionData>(
                    "Draw Card (You forgot to say 'UNO!')",
                    MEMBER_FUNC_REF(PlayerOptionDrawCard)
                )
            });
            matchUI.currentSelectedIndex = 0;
            playerSaidUno = false;
        }
        else {
            for (size_t i = 0; i < player_deck.size(); i++)
            {
                auto& card = player_deck[i];
                matchUI.AddUserOptions({
                    std::make_shared<UserOptionData>(
                        card.ColoredDescription(),
                        SIMPLE_FUNC_REF(use_card_func, player, player_deck, i, true)
                    )
                    });
            }
            matchUI.AddUserOptions({
                std::make_shared<UserOptionData>(
                    "Draw Card",
                    MEMBER_FUNC_REF(PlayerOptionDrawCard)
                )
                });

            if (duelists[0]->hand->deck.size() == 2 && playerSaidUno == false)
            {
                matchUI.AddUserOptions({
                    std::make_shared<UserOptionData>(
                        "UNO!",
                        MEMBER_FUNC_REF(SayUno)
                    )
                    });
            }
        }
        // As options reset, selection was lost
        //
        matchUI.currentSelectedIndex = lastOptionIndex;
        matchUI.ShowOptions();

        // Start duelists turns
        //
        if (playerTurnActionDone) {

            DrawTable(&matchUI, 0, false);
            if (skip == false) {
                WaitForAnyKeyOrDelay(&matchUI);
            }
            if (wildPlayed == false) {

            }

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
            while (currentDuelistIndex != 0 && duelists[0]->hand->deck.size() != 0) {

                // delay
                //
                if (skip) {
                    skip = false;
                    currentDuelistIndex += dir;
                    if (currentDuelistIndex >= duelists.size())
                        currentDuelistIndex = 0;
                    continue;
                }

                DrawTable(&matchUI, currentDuelistIndex, false);
                WaitForAnyKeyOrDelay(&matchUI);
                if (wildPlayed)
                    break;

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
                    if (cardsToBuy_4 > 0) {
                        for (size_t i = 0; i < cardsToBuy_4; i++) {
                            DrawCard(duelists[currentDuelistIndex]->hand->deck);
                        }
                        cardsToBuy_4 = 0;
                    }
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

                DrawTable(&matchUI, currentDuelistIndex, false);
                WaitForAnyKeyOrDelay(&matchUI);
                if (wildPlayed)
                    break;

                currentDuelistIndex += dir;
                if (currentDuelistIndex >= duelists.size())
                    currentDuelistIndex = 0;
            }
        }

        // End duelists turns

        //aaa
        if (wildPlayed) {
            playerTurn = false;
            wildPlayed = false;
        }
        else
            playerTurn = skip == false;
        skip = false;

        // UNO Busted! (player didnt say uno)
        //
        if (duelists[0]->hand->deck.size() == 1 
            && playerSaidUno == false 
            && playerTurnActionDone == false) {

            matchUI.ClearOptions();
            auto& player = duelists[0];
            auto& player_deck = player->hand->deck;

            matchUI.AddUserOptions({
                std::make_shared<UserOptionData>(
                    "Draw Card (You forgot to say 'UNO!')",
                    MEMBER_FUNC_REF(PlayerOptionDrawCard)
                )
            });
            matchUI.currentSelectedIndex = 0;
            playerSaidUno = false;
            matchUI.ShowOptions();
        }

        DrawTable(&matchUI, 0, false);
        
        if (winner == nullptr && playerTurn) {
            // Skip? Player 
            //
            if (skip && wildPlayed) {
                skip = false;
                playerTurnActionDone = true;
            }
            // Player Action
            //
            else {
                matchUI.ReadOptionAndExecute();
                lastOptionIndex = matchUI.currentSelectedIndex;
                DrawTable(&matchUI, 0, false);
                //SafeGetChar();
                if (winner)
                    winnerIndex = 0;
            }
        }

        // Show winner
        //
        if (winner != nullptr) {
            matchUI.SetUserOptions({});
            DrawTable(&matchUI, winnerIndex, true);
            matchUI.SetUserMessage(
                "\n"
                " GAME OVER !"
                "\n" 
                " Winner is " + winner->name
            );
            WaitForAnyKey();
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
