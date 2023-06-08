
// Created by stefam on 14. 5. 2023.
//

#include "Game.h"
#include "InputHandler.h"


using namespace std;

Game::Game() :
    finished(false),
    currentMode(nullptr)
    {}

void Game::run() {
    Game::initialize();
    InputHandler::enableNonCanonicalMode();
    while(!finished) {
        Game::gameLoop();
    }
}

void Game::initialize() {
    currentMode = std::make_unique<MainMenu>();
    currentMode->render();
}

bool Game::gameLoop() {
    while(true) {
        auto startTick = std::chrono::high_resolution_clock::now();
        if(currentMode->shouldEnd() || finished){
            finished = true;
            InputHandler::disableNonCanonicalMode();
            system("clear");
            break;
        }
        if (currentMode->shouldChange()) {
            currentMode = std::move(currentMode->getNextMode());
            currentMode->render();
            break;
        }
        currentMode->handleInput();
        if(currentMode->shouldUpdate()){
            currentMode->render();
        }
        currentMode->tickAction();
        auto endTick = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTick - startTick);
        if (elapsed < DEFAULT_TICK) {
            std::this_thread::sleep_for(DEFAULT_TICK - elapsed);
        }
    }
    return true;
}


