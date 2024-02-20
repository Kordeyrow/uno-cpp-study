#include "UnoSceneManager.h"
#include "MainMenuScene.h"

UnoSceneManager::UnoSceneManager() {
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