//
// Created by stefam on 14. 5. 2023.
//

#pragma once


#include "Game.h"

class Game;

class Renderer {
public:
    Renderer(){};
    void clearScreen(); //Sends remove command to the terminal

private:
    void render(const Game &game) const;
    void renderPlayingMode(const Game &game) const;
    void renderMenuMode(const Game &game) const ;
    void renderSettingsMode(const Game &game) const;
    void renderRankingTable(const Game &game) const;
    static char getCellRepresentation(const CellType &cellType) ;
};

