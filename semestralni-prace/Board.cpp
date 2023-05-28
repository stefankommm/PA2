//
// Created by stefam on 14. 5. 2023.
//

#include "Board.h"
#include "config.h"

void Board::initialize() {
    bool hasPacman = false;
    int ghostSpawns = 0;
    vector<pair<int,int>> coinPosition;

    for (int y = 0; y < grid[0].size(); y++) {
        for (int x = 0; x < grid.size(); x++) {
            switch (grid[x][y]) {
                case CellType::CoinPoints:
                case CellType::CoinReverseEating:
                    coinPosition.emplace_back(x,y);  // Swap x and y
                    break;
                case CellType::GhostSpawn:
                    ghostSpawns++;
                    break;
                case CellType::PacmanSpawn:
                    if (hasPacman)
                        throw std::runtime_error("Na ploche nemoze byt viac ako jeden pacman");
                    hasPacman = true;
                    createPacman(x,y);  // Swap x and y
                    break;
                case CellType::Teleport:
                    if (y == 0) {
                        if (grid[grid.size() - 1][y] == CellType::Teleport) {
                        } else if (grid[x][grid[0].size() - 1] == CellType::Teleport) {  // Swap x and y
                        } else {
                            throw std::runtime_error("Teleport musi smerovat zo strany na stranu");
                        }
                    }
                    teleports.emplace();
                    teleports.emplace();
                    break;
                default:
                    break;
            }
        }
        std::cout << std::endl;
    }
}



void Board::render() {
    for (int y = 0; y < grid[0].size(); y++) {
        for (int x = 0; x < grid.size(); x++) {
            switch (grid[x][y]) {
                case CellType::Empty:
                    std::cout << " ";
                    break;
                case CellType::Wall:
                    std::cout << rang::bgB::blue << " " << rang::style::reset;
                    break;
                case CellType::CoinPoints:
                    std::cout << "·";
                    break;
                case CellType::Pacman:
                    if (pacman_ptr != nullptr && x == pacman_ptr->getPosition().first && y == pacman_ptr->getPosition().second)
                        std::cout << rang::fgB::yellow << pacman_ptr->getSymbol() << rang::style::reset;
                    else
                        std::cout << " ";
                    break;
                case CellType::CoinReverseEating:
                    std::cout << rang::fgB::magenta << "C" << rang::style::reset;
                    break;
                case CellType::Border:
                    std::cout << rang::bg::gray << rang::style::bold << " " << rang::style::reset;
                    break;
                case CellType::GhostSpawn:
                    std::cout << rang::fgB::red << "G" << rang::style::reset;
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
}

void Board::rotatePacman(Direction dir) {
    pacman_ptr->setDirection(dir);

}

bool Board::hasGhostAt(pair<int, int> pos) const {
    for(const auto & c : characters){
        if(c->getPosition() == pos){
            return true;
        }
    }
    return false;
}

void Board::updatePositions(unsigned long long tick) {
    if(tick % DEFAULT_PACMAN_SPEED == 0) {
       pacman_ptr->move();
    }



}

bool Board::isShouldUpdate() const {
    return shouldUpdate;
};
