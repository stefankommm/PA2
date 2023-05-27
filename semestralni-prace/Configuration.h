//
// Created by stefam on 14. 5. 2023.
//

#pragma once


#include <string>
#include "Board.h"
#include <fstream>

using namespace std;
class Configuration {
public:
    explicit Configuration(string filename) {};
    static std::vector<std::vector<CellType>> loadMapFromFile(const string & mapLocation);

private:
};

