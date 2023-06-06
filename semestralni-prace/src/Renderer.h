//
// Created by stefam on 14. 5. 2023.
//

#pragma once



#include <iostream>
#include <cmath>

#include "../libraries/rang.hpp"
#include "../Modes/Mode.h"

class Board;
class Mode;
class Game;
class Settings;
class MainMenu;
class PromptMap;
class ChooseDifficulty;
class RankingTable;
class PromptName;


class Renderer {
public:
    static void clearScreen(); //Sends remove command to the terminal
    static void renderBoard(const Board & board) ;
    static void renderBoardStats(const Board & board) ;
    static void renderSettings(const Settings & game);
    static void renderMainMenu(const MainMenu & menu);
    static void renderRankingTable(const RankingTable & menu);
    static void renderPromptMap(const PromptMap & map);
    static void renderChooseDifficulty(const ChooseDifficulty & map);
    static void renderPromptName(const PromptName & prompt);
private:
};

