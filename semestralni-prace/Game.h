//
// Created by stefam on 14. 5. 2023.
//

#pragma once

#include <memory>
#include <chrono>
#include "Configuration.h"
#include "Modes/Mode.h"
#include "Board.h"
#include "Renderer.h"

class Renderer;
class Configuration; // corrected the typo here
class Board;
class Mode;
class InputHandler;

class Game {
public:
    Game();
    void run();
    void initialize();
    bool gameLoop();
    void setResult(const std::string &player, int score);
    void requestEnd();


private:
    bool finished;
    std::unique_ptr<Configuration> config;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Mode> currentMode;
    std::unique_ptr<InputHandler> inputHandler;
    std::chrono::high_resolution_clock clock;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTickTime;
    std::vector<std::pair<std::string, int>> results;
public:
    const vector<std::pair<std::string, int>> &getResults() const;

};
