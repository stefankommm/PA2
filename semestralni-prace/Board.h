//
// Created by stefam on 14. 5. 2023.
//

#pragma once
#include <utility>
#include <vector>
#include "Character.h"
#include <iostream>
#include "rang.hpp"
class Character;

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
class Board {
public:
    Board(std::vector<std::vector<CellType>> initialGrid, int difficulty) : grid(std::move(initialGrid)), initialized(false){
        initialize();
    };

    void createPacman(int x, int y) {

    }

    void createGhost(int x, int y) {

    }


    void initialize() {
        bool hasPacman = false;
        int ghostSpawns = 0;
        vector<pair<int,int>> coinPosition;
//        multimap<pair<int,int>,pair<int,int>> teleports;

        for (int y = 0; y < grid[0].size(); y++) {
            for (int x = 0; x < grid.size(); x++) {
                switch (grid[x][y]) {
                    case CellType::CoinPoints:
                    case CellType::CoinReverseEating:
                        coinPosition.emplace_back(x,y);
                        break;
                    case CellType::GhostSpawn:
                        createGhost(x,y);
                        ghostSpawns++;
                        break;
                    case CellType::PacmanSpawn:
                        if(hasPacman)
                            throw std::runtime_error("Na ploche nemoze byt viac ako jeden pacman");
                        hasPacman = true;
                        createPacman(x,y);
                        break;
                    case CellType::Teleport:
                        if(y!=0 || y!=grid[0].size() || x!=0 || x!=grid.size())
                            throw std::runtime_error("Teleport musi smerovat zo strany na stranu");
                        std::cout << "T";
                        break;
                    default:
                        break;
                }
            }
            std::cout << std::endl;
        }
    }

    void render() {
        for(const auto & col : grid){
            for(const auto & row : col){
                switch(row){
                    case CellType::Empty:
                        std::cout << " ";
                        break;
                    case CellType::Wall:
                        std::cout << rang::bgB::blue << " " << rang::style::reset;
                        break;
                    case CellType::CoinPoints:
                        std::cout << "·";
                        break;
                    case CellType::CoinReverseEating:
                        std::cout << rang::fgB::magenta << "C" << rang::style::reset;
                        break;
                    case CellType::Border:
                        std::cout << rang::bg::gray << rang::style::bold <<  " " << rang::style::reset;
                        break;
                    case CellType::GhostSpawn:
                        std::cout << rang::fgB::red <<  "G" << rang::style::reset;
                        break;
                    case CellType::PacmanSpawn:
                        std::cout << rang::fgB::yellow << "▸" << rang::style::reset;
                        break;
                    case CellType::Teleport:
                        std::cout << "T";
                        break;
                }
            }
            std::cout << std::endl;
        }
    };

    void update();

    void buildTeleports(const std::vector<std::vector<CellType>>& initialGrid);

    void addCharacter(std::unique_ptr<Character> character);

    bool hasCharacterAt(int x, int y) const;

    bool hasCoinAt(int x, int y) const;

    bool hasTeleportAt(int x, int y) const;

    bool hasCoinReverseEating(int x, int y) const;

    void collectCoinAt(int x, int y);

    void collectReverseEatingCoinAt(int x, int y);

    void teleport();

    int getScore() const;


//    Pacman* getPacman() const {
//        return pacman;
//    }
    int getHeight() const;
    const int getWidth() const ;
    int hasCharactersAt(int x, int y) const;
    std::unique_ptr<Character> getCharacterAt(int x, int y) const;
    char getCellRepresentation(CellType type) const;
    CellType getCellTypeAt(int x, int y) const;




private:
//    Pacman* pacman = nullptr;
    int score;
    bool initialized;
    std::vector<std::vector<CellType>> grid;
    std::vector<std::unique_ptr<Character>> characters;
//    multimap<pair<int,int>,pair<int,int>> teleports; // 1<->1, 2<->2, 3<->3; 1,2,3 represents teleports on the map
};
