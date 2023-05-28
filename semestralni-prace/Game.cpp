
// Created by stefam on 14. 5. 2023.
//

#include "Game.h"
#include <iostream>
#include <memory>
#include <thread>

const std::chrono::milliseconds DEFAULT_TICK(50); // 1000ms / 20 = 50ms // 20FPS

using namespace std;

Game::Game() : finished(false),config(nullptr), renderer(nullptr), currentMode(nullptr), inputHandler(nullptr){}



void Game::run() {
    Game::initialize();
    inputHandler->enableNonCanonicalMode();
    while(!finished) {
        Game::gameLoop();
    }
}


void Game::initialize() {
    //prompt user for map file he wants to use
    //load map using configuration
//    config = std::make_unique<Configuration>("test");
    //
//    board = std::make_unique<Board>(config->loadMapFromFile());
    renderer = std::make_unique<Renderer>();
    currentMode = std::make_unique<MainMenu>();
    inputHandler = std::make_unique<InputHandler>();
    results = {{"Marek",100},{"Zbirka",80},{"Peter",40},{"Jan", 30}};
    currentMode->render(renderer.get(),this);

}

bool Game::gameLoop() {
    while(true) {
        auto startTick = std::chrono::high_resolution_clock::now();
        if(finished){
            break;
        }
        if (currentMode->shouldChange()) {
            currentMode = std::move(currentMode->getNextMode());
            currentMode->render(renderer.get(),this);
            break;
        }
        currentMode->handleInput(inputHandler.get(), this);
        if(currentMode->shouldUpdate()){
            currentMode->render(renderer.get(),this);
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

const vector<std::pair<std::string, int>> &Game::getResults() const {
    return results;
}

void Game::requestEnd() {
    inputHandler->disableNonCanonicalMode();
    renderer->clearScreen();
    finished = true;
}

