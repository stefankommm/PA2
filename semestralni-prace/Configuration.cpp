//
// Created by stefam on 14. 5. 2023.
//

#include <list>
#include "Configuration.h"

std::vector<std::vector<CellType>> Configuration::loadMapFromFile(const std::string& mapLocation) {
    std::ifstream file(mapLocation);
    if (!file.is_open()) {
        throw std::runtime_error("Neviem otvorit subor: " + mapLocation);
    }

    std::vector<std::vector<CellType>> newGrid;

    std::string line;
    while (std::getline(file, line)) {
        std::vector<CellType> row;
        for (int x = 0; x < line.size(); x++) {
            char c = line[x];
            CellType cellType;
            switch (c) {
                case 'W':
                    cellType = CellType::Wall;
                    break;
                case '-':
                    cellType = CellType::CoinPoints;
                    break;
                case 'C':
                    cellType = CellType::CoinReverseEating;
                    break;
                case 'B':
                    cellType = CellType::Border;
                    break;
                case 'G':
                    cellType = CellType::GhostSpawn;
                    break;
                case 'P':
                    cellType = CellType::PacmanSpawn;
                    break;
                case 'T':
                    cellType = CellType::Teleport;
                    break;
                case ' ':
                    cellType = CellType::Empty;
                    break;
                case ',':
                    break;
                case '\0':
                    break;
                case '\n':
                    break;
                default:
                    throw std::runtime_error("Subor Obsahuje neocakavane znaky");
            }
            if (c != ',')
                row.push_back(cellType);
        }
        newGrid.push_back(row);
    }

    if (!file.eof()) {
        throw std::runtime_error("Subor je poskodeny: " + mapLocation);
    }

    if (newGrid.size() < 10 || newGrid[0].size() < 10) {
        throw std::runtime_error("Subor je poskodeny: " + mapLocation);
    }

    for (const auto& i : newGrid) {
        if (i.size() != newGrid[0].size()) {
            throw std::runtime_error("Pocet stlpcov v riadku sa nezhoduju: " + mapLocation);
        }
    }

    // Transpose the grid to convert it to x, y format
    std::vector<std::vector<CellType>> transposedGrid(newGrid[0].size(), std::vector<CellType>(newGrid.size()));
    for (int y = 0; y < newGrid.size(); y++) {
        for (int x = 0; x < newGrid[0].size(); x++) {
            transposedGrid[x][y] = newGrid[y][x];
        }
    }

    return transposedGrid;
}