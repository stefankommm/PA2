//
// Created by stefam on 14. 5. 2023.
//

#pragma once
#include <utility>
#include <vector>
#include "Character.h"
#include <iostream>
#include "rang.hpp"
#include <map>
#include "config.h"

class Character;
class Pacman;

using namespace std;
enum class CellType {
    Empty,
    Wall,
    Border,
    CoinPoints,
    CoinReverseEating,
    GhostSpawn,
    PacmanSpawn,
    Teleport,
    Pacman,
    Ghost,
};



//
//class CellNode {
//public:
//
//private:
//    vector<Character *> characters;
//    bool accessible;
//    bool hasCoin = false;
//    bool hasCoinReverseEating = false;
//    bool isTeleport = false;
//};
//
//


class Board {
public:
    Board(std::vector<std::vector<CellType>> initialGrid, int difficulty) : grid(std::move(initialGrid)), counter(0){
        initialize();
    };

    void createPacman(int x, int y) {
        pacman_ptr = make_unique<Pacman>(x,y, *this);
    }

//    void createGhost(int x, int y) {
//        characters.push_back(make_unique<Ghost>(x,y, make_unique<SmartMovement>()));
//    }


    void initialize();

    void render();

    void updatePositions(unsigned long long tick);

    void rotatePacman(Direction dir);

    bool hasGhostAt(pair<int,int>) const;

    bool hasCoinAt(pair<int,int>) const;
    bool hasTeleportAt(pair<int,int>) const;
    bool hasCoinReverseEating(pair<int,int>) const;
    void collectCoinAt(pair<int,int>);
    void collectReverseEatingCoinAt(pair<int,int>);
    pair<int,int> teleport(pair<int,int>);
    int getHeight() const;
    const int getWidth() const ;
    int hasCharactersAt(int x, int y) const;
    std::unique_ptr<Character> getCharacterAt(int x, int y) const;
    char getCellRepresentation(CellType type) const;
    CellType getCellTypeAt(int x, int y) const;




private:
    bool shouldUpdate;
public:
    bool isShouldUpdate() const;

private:
    unique_ptr<Pacman> pacman_ptr = nullptr;
    unsigned int counter;
    int score;
    std::vector<std::vector<CellType>> grid;
    std::vector<std::unique_ptr<Character>> characters;
    multimap<pair<int,int>,pair<int,int>> teleports; // 1<->1, 2<->2, 3<->3; 1,2,3 represents teleports on the map
    friend class Pacman;
};
