/**
 * @file Game.h
 * @author Marek Stefanko
 * @date 14. 5. 2023
 */

#pragma once

#include <memory>
#include <chrono>
#include <vector>
#include "../Modes/Mode.h"

class Renderer;
class Configuration;
class Board;
class Mode;
class InputHandler;

/**
 * @class Game
 * @brief Main class which handles the main logic of the application
 *
 * This class is handling the main game logic. Importantly it handles the game loop which takes care
 * of switching Modes the application can be in. F.E: MainMenu, Playing, RankTable,...
 * It also handles a tick of 20FPS which the Modes take care of
 */
class Game {
public:
    /**
     * @brief Default constructor for the Game class.
     *
     * Sets up initial game state.
     */
    Game();
    /**
     * @brief Initializes and starts the main game loop
     *
     * Function to be called after Constructor
     */
    void run();

private:
    void initialize();
    bool gameLoop();


    bool finished;
    std::unique_ptr<Mode> currentMode;
    std::chrono::high_resolution_clock clock;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTickTime;
    std::vector<std::pair<std::string, int>> results;
};
