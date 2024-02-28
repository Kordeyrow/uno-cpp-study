#include <iostream>
#include <windows.h>
#include "Uno/UnoSceneManager.h"
#include "Uno/Scenes/MainMenuScene.h"

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100; // The size of the cursor, from 1 to 100. The size is a percentage of the character cell.
    info.bVisible = FALSE; // Set the cursor visibility to FALSE
    SetConsoleCursorInfo(consoleHandle, &info);
}

UnoSceneManager::UnoSceneManager() {
    hideCursor();
    currentScene = std::make_shared<MainMenuScene>();
    currentScene->Init();
}

auto UnoSceneManager::Run() -> void 
{
    while (currentScene) {
        auto nextScene = currentScene->Run();

        // Close app
        if (nextScene == nullptr) {
            break;
        }

        // Change scene
        if (nextScene != currentScene) {
            currentScene = nextScene;
            currentScene->Init();
        }
    }
}
