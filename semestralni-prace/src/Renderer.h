//
// Created by stefam on 14. 5. 2023.
//

#pragma once

#include <iostream>
#include <cmath>

#include "../libs/rang.hpp"
#include "Modes/Mode.h"



/**
 * @brief Class responsible for rendering the Modes to the COUT
 * Handles rendering of the Modes to the screen.
 */
class Renderer {
public:
    /**
     * @brief Function which is supposed to call appropriate command to clear the screen.
     * Function calls system function to the terminal, which handles clearing the screen.
     */
    static void clearScreen();
    /**
     * Handles rendering the Board on the screen which holds the information about current state of the game
     * @brief Handles Rendering the grid of the board class.
     * @param board
     */
    static void renderBoard(const Board & board) ;
    /**
      * Handles rendering the Stats of the Board on the screen which holds the information about additional information the Board has
      * @brief Handles Rendering the other stats of the board class.
      * @param board
      */
    static void renderBoardStats(const Board & board) ;
    /**
     * Handles rendering the MainMenu object
     * @param menu Reference to the MainMenu object
     */
    static void renderMainMenu(const MainMenu & menu);
    /**
     * Handles rendering the RankingTable object
     * @param menu reference to the RankingTable
     * @param results Reference to the results which holds information about user's name and score
     */
    static void renderRankingTable(const RankingTable & menu, const  std::vector<std::pair<std::string, int>> & results);
    /**
     * Handles rendering the PromptMap object
     * @param map Reference to the object ProomptMap
     */
    static void renderPromptMap(const PromptMap & map);
    /**
    * Handles rendering the ChooseDifficulty object
    * @param map Reference to the object ChooseDifficulty
    */
    static void renderChooseDifficulty(const ChooseDifficulty & map);
    /**
    * Handles rendering the PromptName object
    * @param prompt Reference to the object PromptName
    */
    static void renderPromptName(const PromptName & prompt);
private:
};

