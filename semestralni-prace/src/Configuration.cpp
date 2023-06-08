//
// Created by stefam on 14. 5. 2023.
//
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include "Configuration.h"

#include "../libs/nlohmann/json.hpp"


/**
 * @brief Loads a map from a file and converts it into a 2D grid of CellTypes.
 * @param mapLocation The location of the map file to load.
 * @return The 2D grid representing the map.
 * @throws std::runtime_error If the file cannot be opened, the map is invalid, or any other error occurs.
 */
std::vector<std::vector<CellType>> Configuration::loadMapFromFile(const std::string& mapLocation) {
    std::ifstream file(mapLocation);
    if (!file.is_open()) {
        throw std::runtime_error("Neviem otvorit subor: " + mapLocation);
    }
    std::vector<std::vector<CellType>> newGrid;
    int pacmanSpawns = 0;
    int ghostSpawns = 0;
    int coins = 0;
    int borders = 0;
    int teleports = 0;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<CellType> row;
        for (const char & c : line) {
            CellType cellType;
            switch (c) {
                case 'W':
                    cellType = CellType::Wall;
                    break;
                case '-':
                    cellType = CellType::CoinPoints;
                    coins++;
                    break;
                case 'C':
                    cellType = CellType::CoinReverseEating;
                    coins++;
                    break;
                case 'B':
                    cellType = CellType::Border;
                    borders++;
                    break;
                case 'G':
                    cellType = CellType::GhostSpawn;
                    ghostSpawns++;
                    break;
                case 'P':
                    cellType = CellType::PacmanSpawn;
                    pacmanSpawns++;
                    break;
                case 'T':
                    cellType = CellType::Teleport;
                    teleports++;
                    break;
                case 'S':
                    cellType = CellType::Spawn;
                    break;
                case ' ':
                    cellType = CellType::Empty;
                    break;
                case ',':
                case '\0':
                case '\n':
                    break;
                default:
                    throw std::runtime_error("Subor Obsahuje neocakavane znaky"+ mapLocation);
            }
            if (c != ',')
                row.push_back(cellType);
        }
        newGrid.push_back(row);
    }

    if(pacmanSpawns != 1){
        throw std::runtime_error("Mapa musi mat prave jeden spawn pre Pacmana - 'P' "+ mapLocation);
    }

    if(coins == 0){
        throw std::runtime_error("Mapa nema ziadnu z minci. Pacman automaticky vyhral - pridajte '-' alebo 'C'"+ mapLocation);
    }

    if(ghostSpawns == 0){
        throw std::runtime_error("Mapa musi mat aspon jeden spawn pre Ducha - 'G'"+ mapLocation);
    }

    for (const auto & i : newGrid) {
        if (i[0] != CellType::Wall && i[0] != CellType::Teleport) {
            throw std::runtime_error("Prvy stlpec musi obsahovat stenu alebo teleport "+ mapLocation);
        }
    }

    for (const auto & i : newGrid) {
        if (i[i.size() - 1] != CellType::Wall && i[i.size() - 1] != CellType::Teleport) {
            throw std::runtime_error("Posledny stlpec musi obsahovat stenu alebo teleport "+ mapLocation);
        }
    }

    // Check first row
    for (const auto & j : newGrid[0]) {
        if (j != CellType::Wall && j != CellType::Teleport) {
            throw std::runtime_error("Prvy riadok musi obsahovat stenu alebo teleport "+ mapLocation);
        }
    }

    // Check last row
    for (const auto & j : newGrid[newGrid.size() - 1]) {
        if (j != CellType::Wall && j != CellType::Teleport) {
            throw std::runtime_error("Prvy riadok musi obsahovat stenu alebo teleport "+ mapLocation);
        }
    }

    if (!file.eof()) {
        throw std::runtime_error("Subor je poskodeny: " + mapLocation);
    }

    if (newGrid.size() < 10 || newGrid[0].size() < 10) {
        throw std::runtime_error("Mapa nesplna minimalne rozmery 10x10: " + mapLocation);
    }

    for (const auto& i : newGrid) {
        if (i.size() != newGrid[0].size()) {
            throw std::runtime_error("Pocet stlpcov v riadku sa nezhoduju: " + mapLocation);
        }
    }

    std::vector<std::vector<CellType>> transposedGrid(newGrid[0].size(), std::vector<CellType>(newGrid.size()));
    for (int y = 0; y < (int)newGrid.size(); y++) {
        for (int x = 0; x < (int)newGrid[0].size(); x++) {
            transposedGrid[x][y] = newGrid[y][x];
        }
    }

    return transposedGrid;
}


LevelSettings Configuration::loadConfiguration(int difficulty) {
    std::ifstream configFile("config.json");

    if (!configFile.is_open()) {
        std::cerr << "Error: Failed to open config file." << std::endl;
        throw std::runtime_error("Konfiguracny subor config.json neexistuje: ");
    }

    nlohmann::json config;
    try {
        configFile >> config;
    } catch (nlohmann::json::parse_error &e) {
        throw std::runtime_error("JSON Chyba pri parsovani: ");;
    }
    configFile.close();

    LevelSettings loaded{};
    string pick;
    switch(difficulty){
        default:
        case 0:
            pick = "EASY";
            break;
        case 1:
            pick = "MEDIUM";
            break;
        case 2:
            pick = "HARD";
            break;
    }

    try {
        loaded.ghosts = config.at("difficulty").at(pick).at("GHOSTS");
        loaded.ghostSpeed = config.at("difficulty").at(pick).at("GHOST_SPEED");
        loaded.pacmanSpeed = config.at("difficulty").at(pick).at("PACMAN_SPEED");
        loaded.reverseEatingLength = config.at("difficulty").at(pick).at("REVERSE_EATING_LENGTH");
    } catch (nlohmann::json::type_error &e) {
        throw std::runtime_error("JSON Type Chyba: ");;

    } catch (nlohmann::json::out_of_range &e) {
        throw std::runtime_error("JSON mimo indexu: ");;
    }

    return loaded;
}
