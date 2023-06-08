//
// Created by stefam on 14. 5. 2023.
//

#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "constants.h"

using namespace std;

/**
 * @brief The Configuration class handles the loading and parsing of configuration data for the game.
 *
 * This class provides methods to load the game map from a file, as well as to load the game settings for different
 * difficulty levels from a configuration file.
 *
 * The Configuration class ensures that the loaded map and settings meet certain requirements and throws appropriate
 * exceptions if any errors are encountered during the loading process.
 */
class Configuration {
public:
    /**
      * @brief Loads a map from a file and converts it into a 2D grid of CellTypes.
      * @param mapLocation The location of the map file to load.
      * @return The 2D grid representing the map.
      * @throws std::runtime_error If the file cannot be opened, the map is invalid, or any other error occurs.
      */
    static std::vector<std::vector<CellType>> loadMapFromFile(const string & mapLocation);

    /**
     * @brief Loads the configuration settings for a specific difficulty level.
     * @param difficulty The difficulty level.
     * @return The loaded LevelSettings for the specified difficulty.
     * @throws std::runtime_error If the configuration file cannot be opened, parsed, or if any required settings are missing.
     */
    static LevelSettings loadConfiguration(int difficulty);
};

